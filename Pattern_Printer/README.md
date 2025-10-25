# 🧩 PatternPrinter — Modern C Project

A **modular pattern generator in C**, designed for portfolio and freelance showcases. This project prints various ASCII art patterns (pyramids, diamonds, Pascal’s triangle, and more) with clean structure, modular design, and cross-platform build support.

---

## 🚀 Features

* 🧱 **Modular architecture** — `patterns.h` + `patterns.c` + `main.c`
* ⚙️ **Configurable CLI** — choose pattern, rows, characters, output file, and number mode
* 📤 **File output** — export results directly to `.txt`
* 🧰 **Makefile** — build automation (Linux/macOS/Windows-MinGW compatible)
* 🧾 **Documented API** — simple to extend or integrate into other C projects
* 💼 **Portfolio-ready** — structured, readable, and professional project layout

---

## 📁 Project Structure
'''
PatternPrinter/
├── README.md
├── Makefile
├── build.bat          <- Windows build & run script
├── build.sh           <- macOS/Linux/iOS build & run script
├── src/
│   ├── main.c
│   ├── patterns.c
│   └── patterns.h
├── history/
│   └── output.txt     <- stores all pattern outputs
└── examples/
    └── sample_output.txt

'''

---


---

## 🛠️ Build Instructions

### ✅ Using Makefile (Linux/macOS/iOS)

```bash
make            # Build executable

make run
```


### 🪟 On Windows (manual)

If Makefile isn’t available:

```bash
make

make run
```

---

## 🧩 Supported Patterns

| Pattern          | Flag        | Description                |
| ---------------- | ----------- | -------------------------- |
| Pyramid          | `pyramid`   | Classic star pyramid       |
| Inverted Pyramid | `inverted`  | Upside-down pyramid        |
| Number Pyramid   | `numbers`   | Palindromic number pyramid |
| Pascal Triangle  | `pascal`    | Prints Pascal’s triangle   |
| Diamond          | `diamond`   | Symmetrical diamond shape  |
| Hollow Diamond   | `hollow`    | Outlined diamond pattern   |
| Butterfly        | `butterfly` | Wing-like ASCII art        |

---

## 💡 Program Overview

### 1️⃣ Run the program

---

```bash
=== Pattern Printer ===
1. Pyramid
2. Inverted Pyramid
3. Number Pyramid
4. Pascal Triangle
5. Diamond
6. Hollow Diamond
7. Butterfly
8. Exit
Enter choice (1-8):
```

---

## ⚙️ Future Enhancements

* 🧪 Unit testing with CUnit or custom test harness
* 🌈 Add colorized output via ANSI escape codes
* 🖥️ Add GUI frontend or WebAssembly build for browser demo
* ⚡ Integrate GitHub Actions for automated builds
* 🪶 Add ASCII-art saving as `.png` or `.html`

---

## 📜 License

This project is licensed under the **MIT License** — feel free to use, modify, and distribute.

---

### 💻 Author

**Developed by:** Mr-Anonymous-Guy**
📧 Contact: mr.anonymous071105@gmail.com
🌐 GitHub: [github.com/Mr-Anonymous-Guy]

---


