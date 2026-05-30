# NovaShell

NovaShell is a production-grade UNIX-like shell implemented in C. It features POSIX-compliant process orchestration, advanced AST-based parsing, job control, terminal session management, signal orchestration, and extensible execution pipelines.

## Engineering Overview

Designed as a serious systems-level command interpreter, NovaShell draws inspiration from shells like Bash, Zsh, Fish, and Dash. Its architecture prioritizes:
- Separation of concerns
- Deterministic execution
- Reusable parser infrastructure
- Robust UNIX process management
- Strict POSIX compliance

## Shell Architecture

The shell is composed of several modular subsystems:

1. **Terminal Input Layer**: Reads input from the user or script via a Read-Eval-Print Loop (REPL).
2. **Lexer**: Tokenizes the input stream, preserving quoting semantics and variable expansions.
3. **Parser**: Generates an Abstract Syntax Tree (AST) using a recursive descent parser.
4. **Execution Planner & Engine**: Translates the AST into a process graph, configures pipes/redirections, manages descriptor inheritance, and executes processes using `fork`/`execve`.
5. **Job & Terminal Control**: Manages UNIX job control (foreground/background), `tcsetpgrp()`, process groups, and UNIX signals like `SIGINT` and `SIGTSTP`.

## Building and Running

### Linux / WSL

```bash
# Build and run normally
make run

# Build with sanitizers
make sanitize

# Run tests
make test
```

### Windows

The shell leverages advanced POSIX APIs not available natively on Windows. However, we provide a seamless wrapper (`run.bat`) that automatically leverages WSL.

```cmd
run.bat
run.bat debug
run.bat test
```

## Linux Internals Used

- **Processes**: `fork()`, `execve()`, `waitpid()`
- **Process Groups**: `setpgid()`, `getpgid()`, `tcsetpgrp()`
- **Descriptors**: `dup2()`, `pipe()`, `open()`, `close()`
- **Signals**: `sigaction()`, `kill()`

## Future Roadmap

- Extensible scripting engine support
- Advanced autocomplete framework
- Subshell command substitution
- Custom package manager for plugins

---

*NovaShell — Infrastructure-grade UNIX engineering.*
