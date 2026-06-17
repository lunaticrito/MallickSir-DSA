# MathEngine — DSA-Powered Math Expression Engine

## Project Roadmap

### Phase 1 (Current): Terminal Engine
All DSA modules implemented as independent, testable units.
The `core/` layer bridges DSA modules to math logic.
`ui/terminal_ui.cpp` drives everything from the console.

### Phase 2: Graphics Integration (Qt or SFML)
Replace `ui/terminal_ui.cpp` with `ui/qt_ui.cpp` (or `ui/sfml_ui.cpp`).
Zero changes needed in `core/` or `include/` or `src/`.

---

## File Map

```
MathEngine/
├── include/                  # All headers (pure interface, no platform code)
│   ├── array_ops.h           # 1. Array insert/delete
│   ├── stack_queue.h         # 2. Stack & Queue
│   ├── expression_engine.h   # 3. Infix→Postfix/Prefix, evaluation
│   ├── singly_linked.h       # 4. Singly & circular linked list
│   ├── doubly_linked.h       # 5 & 6. Doubly & circular doubly linked list
│   ├── sorting.h             # 7 & 8. All 6 sort algorithms
│   ├── bst.h                 # 9. Binary Search Tree
│   ├── bit.h                 # 10. Binary Indexed Tree (Fenwick)
│   ├── heap.h                # 11. Min/Max Heap + Heap Sort
│   ├── graph.h               # 12. BFS, DFS, Topological Sort
│   └── math_engine.h         # Core engine API (UI-agnostic)
│
├── src/                      # Implementations
│   ├── array_ops.cpp
│   ├── stack_queue.cpp
│   ├── expression_engine.cpp
│   ├── singly_linked.cpp
│   ├── doubly_linked.cpp
│   ├── sorting.cpp
│   ├── bst.cpp
│   ├── bit.cpp
│   ├── heap.cpp
│   ├── graph.cpp
│   └── math_engine.cpp       # Wires DSA modules into math engine logic
│
├── ui/
│   ├── terminal_ui.cpp       # Phase 1: Console UI  ← main() lives here
│   └── qt_ui.cpp             # Phase 2: Add this file, done. (stub shown)
│
└── CMakeLists.txt
```

---

## DSA → Math Engine Mapping

| DSA Concept | Role in Math Engine |
|---|---|
| Array ops | Token buffer management |
| Stack | Shunting-yard operator stack |
| Queue | Output queue for postfix tokens |
| Infix→Postfix | Core expression parsing |
| Postfix eval | Expression evaluation |
| Singly LL | Expression history list |
| Doubly LL | Undo/redo buffer |
| Sorting | Sort variable list, sort results |
| BST | Variable symbol table (name→value) |
| BIT | Running prefix-sum on token stream |
| Heap | Priority queue for operator precedence |
| BFS/DFS | Dependency graph traversal for multi-expr |

---

## Build

All source files live directly in this `v3/` folder (headers and `.cpp` together).

### Windows (PowerShell) — recommended

```powershell
cd v3
.\build.ps1
.\MathEngine.exe
```

Requires **g++** (e.g. from [MSYS2](https://www.msys2.org/) `ucrt64` — `pacman -S mingw-w64-ucrt-x86_64-gcc`).

### CMake (Linux / macOS / Windows with CMake installed)

```bash
cd v3
mkdir build
cd build
cmake ..
cmake --build .
./MathEngine        # Linux/macOS
# or: .\MathEngine.exe   on Windows
```
Run it
cd v3
.\build.ps1
.\MathEngine.exe