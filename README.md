# GC-MALLOC
Simple-GC: A Conservative Garbage Collector for C
A lightweight, Mark-and-Sweep Garbage Collector implemented in C. This project demonstrates manual memory management by "carving" metadata headers into heap allocations and scanning the call stack to identify reachable pointers.

🚀 Features
Custom Allocator: gc_malloc wraps standard malloc to track allocations.
Conservative Stack Scanning: Detects pointers on the stack by walking from the current frame to the stack bottom.
Cross-Platform Stack Discovery: Supports automatic stack boundary detection for Windows (via TIB) and Linux (via environ).
Zero-Dependency: Built using only standard C libraries and platform-specific headers.

🛠 Project Structure
.
├── src/
│   ├── gc.c       # Implementation of marking, sweeping, and stack walking
│   └── gc.h       # Public API and Header struct definition
└── test/
    └── main.c     # Test suite and usage examples

🔨 Compilation
To compile the project, use the following command from the root directory:
gcc src/gc.c test/main.c -I src -o gc_test -Wall -Wextra -O0 -g

Note: The -O0 flag is required. Higher optimization levels may store pointers exclusively in registers, causing the stack scanner to miss them.

💻 Usage
Include gc.h in your project and replace malloc calls with gc_malloc:
#include "gc.h"

int main() {
    // Allocation is tracked automatically
    int *data = (int*)gc_malloc(sizeof(int) * 100);
    
    // You can manually trigger collection or let the threshold handle it
    gc_collect();

    return 0;
}

🔍 How it Works
Metadata Inlining: Every allocation includes a Header struct placed immediately before the returned pointer.

Mark Phase: The GC locates the stack bottom and scans every word on the stack. If a word matches the address of an allocated block, that block is marked as "alive."

Sweep Phase: The GC iterates through the global linked list of allocations; any block not marked as alive is free()'d and removed from the list.

⚠️ Disclaimer
This is a conservative garbage collector. It may occasionally keep memory alive if a non-pointer value (like a large integer) happens to match a heap address.
