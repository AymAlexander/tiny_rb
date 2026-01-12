# Tiny-RB: Lightweight Generic Ring Buffer

## Overview
A lightweight, header-only ring buffer implementation in C with support for both FIFO (queue) and LIFO (stack) operations.

## Features
- Header-only implementation
- Type-safe via template macros
- FIFO (queue) and LIFO (stack)
- Static memory allocation

## Quick Start

### 1. Include Header
```c
#include "tiny_rb.h"
```

### 2. Define Buffer
```c
TRB_RB_DEFINE(int, my_buffer, 10);  // Buffer of 10 integers

TRB_RB_DEFINE(user_type, my_buffer2, 10); // Any type
```

### 3. Basic Usage
```c
// FIFO Mode (Queue)
int value = 42;
trb_fifo_push(my_buffer, &value);  // Push
trb_fifo_pop(my_buffer, &value);   // Pop

// LIFO Mode (Stack)
trb_lifo_push(my_buffer, &value);  // Push
trb_lifo_pop(my_buffer, &value);   // Pop

// Status checks
if (trb_is_empty(my_buffer)) { /* ... */ }
size_t count = trb_size(my_buffer);
```
