# The Arcanum Editor — Win32 Text Editor

A custom desktop text editor built in C++ as a university assignment.

---

## Disclaimer

This was built for a university assignment with highly specific architectural constraints. The primary goal was to enforce strict Object-Oriented Programming principles rather than building a fully-featured, production-ready text editor. The codebase intentionally avoids standard practices like global functions, or standard GUI text-box controls to comply with the assignment rules. It works as a proof-of-concept for custom text rendering and OOP hierarchy, but lacks standard modern text editor features.

---

## What It Does

The application handles text rendering and manipulation entirely from scratch through a custom grid system:

### Data Structure & Hierarchy
- Abandons standard string arrays in favor of a deeply nested class composition
- Structures data strictly as: `Editor` ➔ `Document` ➔ `Page` ➔ `Column` ➔ `Line`
- Completely encapsulates all logic within these object boundaries

### Core Editor Operations
- **Dynamic Text Reflow** — Automatically adjusts and wraps text based on active layout boundaries
- **Layout Configuration** — Granular control over the page grid (max columns, max lines per column, and max characters per line)
- **Search Functionality** — Iterates through the custom data hierarchy to locate specific strings of text
- **Real-time Statistics** — Parses the document grid to actively count words and sentences

---

## How to Run

1. Open in Visual Studio 2019 or later
2. Build as a Windows Desktop Application (ensure Win32 libraries like `User32.lib` and `Gdi32.lib` are linked)
3. Ensure all files are in project directory
4. Compile and run to launch the native Windows GUI

---

## Limitations

- Zero global functions used (strict university constraint)
- No support for rich text formatting (bold, italics, varying fonts)
- No support for copy-pasting
- Relies purely on the Win32 API for window rendering, meaning it is strictly Windows-only
- No support for copying and pasting text
- Can sometimes crash (it is actually rare)
---

## Libraries Used

Only the following standard headers are used as per assignment constraints:

```cpp
#include <windows.h>
#include <iostream>

```
*Built for CS-1004 Object Oriented Programming — FAST NUCES, Spring 2026*
