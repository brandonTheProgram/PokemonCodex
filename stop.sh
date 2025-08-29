#!/bin/bash
set -euo pipefail

# ---- config ----
BACKEND_NAME="PokemonCodex"
NODE_ENTRY="express/Pokedex.js"
# ----------------

echo "Stopping servers..."
if [[ "${OS:-}" == "Windows_NT" ]] || [[ "$(uname -s 2>/dev/null)" =~ (MINGW|MSYS|CYGWIN) ]]; then
    taskkill //F //IM "${BACKEND_NAME}.exe" >/dev/null 2>&1 || true

    powershell.exe -NoProfile -Command "
        \$pattern = [regex]::Escape('${NODE_ENTRY//\//\\}')
        Get-CimInstance Win32_Process |
        Where-Object { \$_.Name -ieq 'node.exe' -and \$_.CommandLine -match \$pattern } |
        ForEach-Object {
            try { Stop-Process -Id \$_.ProcessId -Force -ErrorAction SilentlyContinue } catch {}
        }
    " >/dev/null 2>&1 || true
else
    pkill -f '(^|/)'${BACKEND_NAME}'(\s|$)' >/dev/null 2>&1 || true
    pkill -f "node .*${NODE_ENTRY}" >/dev/null 2>&1 || true
fi

echo "Servers stopped."