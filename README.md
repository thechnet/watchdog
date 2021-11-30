# Watchdog

## Introduction

*Watchdog* will be a C library attempting to automatically warn developers of memory leaks, dangling pointers, buffer overflows, and accidental memory modifications present in their code. Additionally, the library will try to estimate the point of failure in case of segmentation faults and track the real memory usage versus the allocated amount of memory.

One of the main design goals with this library is seamlessness. Therefore, all currently planned features will be enabled automatically by simply including the `watchdog.h` header. Also, as Watchdog will significantly impact your program's performance and greatly increase its memory usage, it is only intended to run in debug builds. 

This project is built from scratch, but not without prior knowledge. I am basing a lot of its functionality on [`memdbg`](https://github.com/thechnet/une/blob/main/src/util/memdbg.c), a similar but less powerful module used during development of my interpreted programming language, [*Une*](https://github.com/thechnet/une).

## Roadmap

> ❌ Yet to start.  
> 🏃 In progress...  
> ✅ Finished and working.  

Status|Work|Notes
-|-|-
🏃|Set up basic infrastructure.|
🏃|Radar (track unfreed allocations)|
❌|Reporter (handle output of information)|
❌|Checkpoints (track control flow)|
❌|Padding (catch buffer overflows)|
❌|Snapshots (catch accidental memory modification)|
❌|Dangling (track dangling pointers)|
❌|Usage (calculate real memory usage)|

## Source Overview

### `checkpoints`

Keeps track of the most recent position at which the program was executing successfully.

### `dangling`

Keeps track of addresses that were previously freed, to warn the user if they are referencing a dangling pointer. Also sets freed pointers to `NULL` after deallocation to cause an immediate crash on use of a dangling pointer.

### `padding`

Handles allocation padding.

### `radar`

Keeps track of unfreed allocations to warn the developer if they are not freeing memory.

### `reporter`

Continously prints status updates to an external file.

### `snapshots`

Keeps copies of memory between operations to catch accidental modifications.

### `usage`

Keeps a copy of the initial data at a memory location to calculate the actual memory usage compared to the allocated amount.

### `private.h`

Contains private global declarations.

### `public.h`

Contains public global declarations.

### `watchdog.c`

Defines general functions such as the initializer.

