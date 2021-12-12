# Watchdog

## Introduction

*Watchdog* aims to become a C library that automatically warns developers of memory leaks, buffer overflows, memory corruption, and dangling pointers present in their code. Additionally, the library will narrow down the crash site on segmentation faults, compare the real memory usage to the allocated amount of memory, and offer manual bounds checking tools.

- One of the main design goals with this library is seamlessness. Therefore, most of its functionality is enabled automatically by simply including the `watchdog.h` header.
- Watchdog significantly impacts your program's performance and greatly increases its memory usage. It should therefore only be enabled in debug builds.
- This project is built from scratch, but not without prior knowledge. I am basing a lot of its functionality on [`memdbg`](https://github.com/thechnet/une/blob/main/src/util/memdbg.c), a similar but less powerful module used during development of my interpreted programming language, [*Une*](https://github.com/thechnet/une).

## Roadmap

Module|Description|Notes|Structure|Features|Stability
-|-|-|-|-|-
Radar|Find memory leaks.<br>`radar.c`, `radar.h`|<ul><li>Remove `.is_native`?</li><li>What do we do if an address is `locate`'d within the padding of an allocation?</li></ul>|🟠|🟢|🟠
Pulse|Narrow down crash sites.<br>`pulse.c`, `pulse.h`|<ul></ul>|🟢|🟢|🟢
Padding|Catch buffer overflows.<br>`padding.c`, `padding.h`|<ul></ul>|🟢|🟢|🟠
Snapshots|Catch memory corruption.<br>`snapshots.c`, `snapshots.h`|<ul></ul>|🟢|🟢|🟠
Archive|Warn about dangling pointers.<br>`archive.c`, `archive.h`|<ul></ul>|🟠|🟢|🟠
Usage|Profile memory usage.<br>`usage.c`, `usage.h`|<ul></ul>|🟠|🟠|🟠
Bounds|Offer manual bounds checking.<br>`bounds.c`, `bounds.h`|<ul><li>Find a solution to the uncertainty when receiving untracked pointers.</li><li>Should we allow negative indices?</li></ul>|🟠|🟠|🟠
Signals|Intercept signals.<br>`signals.c`, `signals.h`|<ul></ul>|🟢|🟢|🟠
Overrides|Automatically inject watchdog into code.<br>`overrides.c`, `overrides.h`, `watchdog.h`|<ul><li>`fopen`, etc.</li><li>Override common functions like `strlen`?</li><li>Check for incoming string literals?</li></ul>|🟠|🟠|🟠
*Ignored*|Manually ignore certain addresses, like constants, when checking for untracked addresses.|<ul></ul>|🔴|🟠|🔴
Reporter|Handle output of information.<br>`reporter.c`, `reporter.h`|<ul><li>Allow users to suppress warnings.</li><li>Highlight non-native allocations in messages.</li><li>Make it clear whether something happened *before* or *after* an overridden operation.</li></ul>|🟠|🟠|🟠
Public Interface|Expose manual functionality to the user.<br>`public.c`, `public.h`|<ul></ul>|🟢|🟠|🟠
Dogshed|Shared functionality and constants.<br>`dogshed.c`, `dogshed.h`|<ul><li>Distribute or collect constants? (e.g. messages, default sizes, etc.)</li></ul>|🟢|🟠|🟢
