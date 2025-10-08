#!/usr/bin/env python3
"""
docs_builder.py
Builds Markdown documentation for a SQLite database.

Features:
- Per-table docs: docs/tables/<Table>.md
- Overview doc:   docs/Database.md (links to per-table docs, summary stats, incoming references)
- Mermaid ER:     docs/er/pokedex.mmd (no Graphviz required; GitHub and many renderers support Mermaid)

Usage:
  python docs_builder.py --db db/Data/Pokedex.db --out docs --samples 15 --no-mermaid   # to skip Mermaid
  python docs_builder.py --db db/Data/Pokedex.db --out docs --samples 15                # Mermaid on by default
"""

import argparse
import os
import sqlite3
from collections import defaultdict

# ---------- CLI ----------
def parse_args():
    p = argparse.ArgumentParser(description="Generate Markdown docs for a SQLite DB (with optional Mermaid ER).")
    p.add_argument("--db", required=True, help="Path to the SQLite database file (e.g., pokemon.db)")
    p.add_argument("--out", default="docs", help="Output directory (default: docs)")
    p.add_argument("--samples", type=int, default=15, help="Sample rows per table (default: 15)")
    p.add_argument("--no-mermaid", action="store_true", help="Skip Mermaid ER diagram generation")
    return p.parse_args()

# ---------- helpers ----------
def fetchall(cur, q, args=()):
    cur.execute(q, args)
    return cur.fetchall()

def md_escape(s: str) -> str:
    if s is None:
        return ""
    return str(s).replace("|", r"\|")

def list_user_tables(cur):
    rows = fetchall(cur, "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY name")
    return [r[0] for r in rows]

def row_count(cur, table):
    return fetchall(cur, f"SELECT COUNT(*) FROM '{table}'")[0][0]

def columns(cur, table):
    # cid, name, type, notnull, dflt_value, pk
    return fetchall(cur, f"PRAGMA table_info('{table}')")

def fks(cur, table):
    # id, seq, table, from, to, on_update, on_delete, match
    return fetchall(cur, f"PRAGMA foreign_key_list('{table}')")

def indexes(cur, table):
    # Newer SQLite: seq, name, unique, origin, partial
    # Older SQLite: name, unique, origin, partial
    return fetchall(cur, f"PRAGMA index_list('{table}')")

def table_markdown(cur, table, sample_rows=15):
    # Row count
    rcount = row_count(cur, table)

    # Columns / FKs / Indexes
    cols = columns(cur, table)
    fklist = fks(cur, table)
    idxs = indexes(cur, table)

    md = []
    md.append(f"# {table}\n")
    md.append(f"**Rows:** {rcount}\n")

    # Columns
    md.append("## Columns\n")
    md.append("| # | name | type | pk | notnull | default |")
    md.append("|---:|-----|------|---:|--------:|---------|")
    for c in cols:
        cid, name, ctype, notnull, dflt_value, pk = c
        md.append(f"| {cid} | {md_escape(name)} | {md_escape(ctype)} | {pk} | {notnull} | {md_escape(dflt_value)} |")

    # FKs
    md.append("\n## Foreign Keys")
    if fklist:
        md.append("| id | seq | from | to | table | on_update | on_delete | match |")
        md.append("|---:|----:|------|----|-------|-----------|-----------|-------|")
        for (id_, seq, ref_table, from_col, to_col, on_upd, on_del, match) in fklist:
            md.append(f"| {id_} | {seq} | {md_escape(from_col)} | {md_escape(to_col)} | {md_escape(ref_table)} | {on_upd} | {on_del} | {match} |")
    else:
        md.append("_(none)_")

    # Indexes
    md.append("\n## Indexes")
    if idxs:
        # normalize row shape for older/newer SQLite
        header = "| seq | name | unique | origin | partial |"
        md.append(header)
        md.append("|----:|------|-------:|--------|---------|")
        for row in idxs:
            if len(row) >= 5:
                seq, name, unique, origin, partial = row[:5]
            else:
                # older shape: name, unique, origin, partial
                seq = ""
                name, unique, origin, partial = row[:4]
            md.append(f"| {seq} | {md_escape(name)} | {unique} | {origin} | {partial} |")
    else:
        md.append("_(none)_")

    # Sample rows
    md.append("\n## Sample Rows")
    try:
        col_names = [c[1] for c in cols]
        sample = fetchall(cur, f"SELECT * FROM '{table}' LIMIT {int(sample_rows)}")
        if sample:
            header = " | ".join([md_escape(c) for c in col_names])
            md.append(f"| {header} |")
            md.append("|" + "|".join(["---"] * len(col_names)) + "|")
            for r in sample:
                md.append("| " + " | ".join(md_escape(str(x)) for x in r) + " |")
        else:
            md.append("_(empty table)_")
    except Exception as e:
        md.append(f"_(error sampling rows: {e})_")

    return "\n".join(md) + "\n"

def write_table_docs(cur, out_dir, tables, sample_rows):
    tables_dir = os.path.join(out_dir, "tables")
    os.makedirs(tables_dir, exist_ok=True)
    for t in tables:
        path = os.path.join(tables_dir, f"{t}.md")
        with open(path, "w", encoding="utf-8") as f:
            f.write(table_markdown(cur, t, sample_rows=sample_rows))
    # small README / TOC
    toc_lines = ["# Database Tables", ""]
    for t in tables:
        toc_lines.append(f"- [{t}](./{t}.md)")
    with open(os.path.join(tables_dir, "README.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(toc_lines) + "\n")

def write_overview_md(cur, out_dir, tables):
    os.makedirs(out_dir, exist_ok=True)
    table_docs_dir = os.path.join(out_dir, "tables")

    lines = []
    lines.append("# Pokémon Codex Database Overview\n")
    lines.append("> Auto-generated summary of the SQLite schema.\n")
    lines.append("## Table of Contents\n")
    for t in tables:
        target = f"tables/{t}.md"
        exists = os.path.exists(os.path.join(table_docs_dir, f"{t}.md"))
        link = target if exists else f"#-no-doc-for-{t}"
        lines.append(f"- [{t}]({link})")
    lines.append("")

    # Summary table
    lines.append("## Tables Summary\n")
    lines.append("| Table | Rows | Columns | Foreign Keys | Indexes |")
    lines.append("|-------|-----:|--------:|-------------:|--------:|")

    # Build incoming references map
    refs_to = defaultdict(list)  # key: table, val: list of (from_table, from_col, to_col)

    for t in tables:
        rcount = row_count(cur, t)
        colcount = len(columns(cur, t))
        fklist = fks(cur, t)
        idxs = indexes(cur, t)
        idxcount = len(idxs)
        lines.append(f"| [{md_escape(t)}](tables/{t}.md) | {rcount} | {colcount} | {len(fklist)} | {idxcount} |")
        for (id_, seq, ref_table, from_col, to_col, on_upd, on_del, match) in fklist:
            refs_to[ref_table].append((t, from_col, to_col))

    # Incoming references
    lines.append("\n## Incoming References (who depends on whom)\n")
    for t in tables:
        incoming = refs_to.get(t, [])
        if not incoming:
            continue
        lines.append(f"**{t}** is referenced by:")
        for (ft, fc, tc) in sorted(incoming):
            lines.append(f"- `{ft}.{fc}` → `{t}.{tc}`")
        lines.append("")

    overview_md = os.path.join(out_dir, "Database.md")
    with open(overview_md, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))
    return overview_md

# ---------- Mermaid ER generation ----------
def make_mermaid_er(cur, tables):
    """
    Generate a Mermaid `erDiagram` string describing tables and FKs.
    We'll approximate cardinalities: ref_table ||--o{ from_table
    and include columns for each entity.
    """
    lines = ["erDiagram"]
    # Entities
    for t in tables:
        cols = columns(cur, t)
        # Mermaid entity block
        lines.append(f"  {t} {{")
        for cid, name, ctype, notnull, dflt, pk in cols:
            label = ctype or "TEXT"
            suffix = " PK" if pk else ""
            lines.append(f"    {label} {name}{suffix}")
        lines.append("  }")
    # Relationships
    for t in tables:
        for (id_, seq, ref_table, from_col, to_col, on_upd, on_del, match) in fks(cur, t):
            # One referenced row can be linked by many in the referencing table:
            # ref_table ||--o{ t : "from_col→to_col"
            rel_label = f"{from_col}→{to_col}"
            lines.append(f"  {ref_table} ||--o{{ {t} : \"{rel_label}\"")
    return "\n".join(lines) + "\n"

def write_mermaid_files(cur, out_dir, tables):
    er_dir = os.path.join(out_dir, "er")
    os.makedirs(er_dir, exist_ok=True)
    er_path = os.path.join(er_dir, "pokedex.mmd")
    content = make_mermaid_er(cur, tables)
    with open(er_path, "w", encoding="utf-8") as f:
        f.write(content)
    return er_path, content

# ---------- main ----------
def main():
    args = parse_args()
    out_dir = args.out
    os.makedirs(out_dir, exist_ok=True)

    conn = sqlite3.connect(args.db)
    cur = conn.cursor()

    tables = list_user_tables(cur)

    # 1) Per-table docs
    write_table_docs(cur, out_dir, tables, sample_rows=args.samples)

    # 2) Overview
    overview_md = write_overview_md(cur, out_dir, tables)

    # 3) Mermaid ER (optional)
    mermaid_path = None
    mermaid_block = ""
    if not args.no_mermaid:
        mermaid_path, mermaid_block = write_mermaid_files(cur, out_dir, tables)

        # Append a Mermaid section to the overview for convenience
        with open(overview_md, "a", encoding="utf-8") as f:
            f.write("\n## ER Diagram (Mermaid)\n")
            f.write("The following Mermaid diagram should render on GitHub and many Markdown viewers:\n\n")
            f.write("```mermaid\n")
            f.write(mermaid_block)
            f.write("```\n")
            f.write("\nYou can also open the full diagram source at ")
            f.write(f"`{os.path.relpath(mermaid_path, start=os.path.dirname(overview_md))}`.\n")

    print(f"Docs built in: {out_dir}")
    if mermaid_path:
        print(f"Mermaid ER:   {mermaid_path}")
    print("Done.")

if __name__ == "__main__":
    main()
