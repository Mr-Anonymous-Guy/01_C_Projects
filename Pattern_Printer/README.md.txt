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

```
PatternPrinter/
├── README.md            <- You are here
├── Makefile
├── src/
│   ├── main.c
│   ├── patterns.c
│   └── patterns.h
└── examples/
    └── sample_output.txt
```

---

## 🧠 How It Works

### PatternOptions Struct (core API)

```c
typedef struct {
    PatternType type;   // Type of pattern (pyramid, diamond, etc.)
    int rows;           // Number of rows
    char fill;          // Fill character
    int numbered;       // Enable number patterns
    FILE *out;          // Output stream (stdout or file)
} PatternOptions;
```

Main entry function:

```c
int print_pattern(const PatternOptions *opt);
```

---

## 🛠️ Build Instructions

### ✅ Using Makefile (Linux/macOS/MinGW)

```bash
make            # Build executable
./pattern_printer -p pyramid -r 5
```

### 🪟 On Windows (manual)

If Makefile isn’t available:

```bash
gcc src/main.c src/patterns.c -o pattern_printer.exe -I src
pattern_printer.exe -p diamond -r 5
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

## 💡 Usage Examples

### 1️⃣ Print Pyramid (default)

```bash
./pattern_printer -p pyramid -r 5
```

```
    *
   ***
  *****
 *******
*********
```

### 2️⃣ Pascal Triangle to File

```bash
./pattern_printer -p pascal -r 6 -o pascal.txt
```

### 3️⃣ Number Pyramid (mirrored)

```bash
./pattern_printer -p numbers -r 5 -n
```

---

## 🧩 Example Output (sample_output.txt)

```
   *
  ***
 *****
*******
```

---

## 📈 Why This Project Attracts Employers

✅ **Readable, structured C code** — professional modular separation and headers.
✅ **Demonstrates CLI + file I/O skills** — commonly used in production-level systems.
✅ **Clean documentation** — instant understanding for hiring managers or clients.
✅ **Expandable design** — new patterns or options can be added with minimal changes.
✅ **Ready for GitHub portfolio** — perfect to showcase low-level programming ability.

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


