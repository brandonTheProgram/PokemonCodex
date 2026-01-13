# Pokémon Codex (Pokedex Project)

A full‑stack Pokédex web app with a **C++17** backend (SQLite) and a **Node.js/Express + EJS** frontend. It renders fast, query‑driven Pokémon pages (types, abilities, evolutions, movesets, locations, etc.) from a curated SQLite database.

---

## 🔗 Live App Link

---

## ✨ Features

* **Search** by name with fast server‑side filtering.
* **Browse by Region** (e.g., Kanto) and view regional Pokémon lists.
* **Pokémon Detail Pages** with:

  * Species data & image
  * Types & type‑effectiveness table
  * Abilities (primary/secondary/hidden)
  * Evolutionary chains (including regional branches & trade/happiness/stone icons)
  * Movesets: Moves gained via leveling up, Technical Machines, and Technical Records
  * Locations/availability (nicely formatted with newlines)
* Clean EJS layouts; dark‑mode friendly styling.

---

## 🛠️ Tech Stack

* **Frontend**: Node.js, Express, EJS, express‑ejs‑layouts, axios
* **Backend**: C++17, CMake, cpp‑httplib, SQLite3 via SQLiteCpp, jsoncpp
* **DB**: SQLite (file‑based)
* **CI/Hosting**: Render (Web Service)

---

## 🚀 Quick Start (Local)

### Prerequisites

* **Node.js** ≥ 18 (LTS recommended)
* **CMake** ≥ 3.20
* A C++17 compiler (GCC/Clang on Linux/macOS, MSVC on Windows)

### 1) Build the backend

Using the portable script:

```bash
./build.sh
```

This configures CMake and builds a **Release** binary into `./build/`.

### 2) Run

```bash
./start.sh
```

* Starts the C++ backend in the background (if present)
* Starts the Express server in the **foreground**
* Open [http://localhost:3000](http://localhost:3000)

> Alternatively, run everything with a single script:
>
> ```bash
> ./all.sh
> ```

---

## 🔧 Configuration (Environment)

The app works out‑of‑the‑box with sensible defaults. These env vars let you tune behavior:

| Variable        | Value    | Default / Notes                                                                                     |
| --------------- | -------- | --------------------------------------------------------------------------------------------------- |
| `LOG_LEVEL`          | 'DEBUG' | The log level for the program                                                  |
| `LOG_DIR`  | 'logs'  | The name of the directory to put the logs                                                |
| `LATEST_LIMIT`   | '5' | The number of Pokemon to display on the home page for lastest Pokemon |
| `STARTING_LIMIT`       | '5'  | The number of Pokemon to display on the home page for each region                                                   |
| `DATABASE_PATH` | 'db/Data/Pokedex.db'  | Path to the SQLite DB                                                  |

---

## 🧪 Tests

* CTest/GTest style unit tests under `/tests`.

## 📜 License

MIT License

Copyright (c) 2025 Brandon Evans

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

---

## 🙏 Acknowledgements

* The information and images are © 2025 Pokémon. © 1995–2025 Nintendo/Creatures Inc./GAME FREAK inc. Pokémon, Pokémon character names, Nintendo Switch, Nintendo 3DS, Nintendo DS, Wii, Wii U, and WiiWare are trademarks of Nintendo. I do not own anything used in this program, just a fan project since I love Pokemon!
* Community resources used during data curation include Serebii and others.

---

## 🤝 Contributing

PRs and issue reports are welcome! Please keep changes consistent with the existing C++ style (C++17, CMake), and EJS/Express conventions on the frontend.
