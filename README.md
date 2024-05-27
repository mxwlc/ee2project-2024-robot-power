# Communications

## Overview

This branch handles the various necessary forms of communication throughout the project. These are:

- Shared memory across processes (C++ and Python);
- Wired transmission from Raspberry Pi to Arduino [INCOMPLETE - WIP];
- Wireless sending to/from Raspberry Pi from/to Laptop [INCOMPLETE].

## /within-rpi/*

This is an implementation of a `SharedMemory` class which creates/connects to a shared memory instance with a specified name and size in bytes. Intended for use on the Raspberry Pi to allow for communication between processes.

### Instructions

#### C++:

Create an instance of the `SharedMemory` class, then the `ptr` member can be used to access the memory (note: `ptr` is left of type `void *` and can be cast as is most convenient).

#### Python:

Import the `.py` file and create an instance of the `SharedMemory` class. Then use the `.getBuffer()` method to get the shared memory as a byte array.

Notes on C++ implementation:
- Intended for static implementation: copy the `.cpp` and `.hpp` into the same directory as your source code, include the `.hpp` in your code and add the `.cpp` as a source file when compiling;

Notes on Python implementation:
- Unintended behaviour occurs when a shared memory object, which connected to an existing shared memory object created either in Python or C++, is destroyed or the program terminates (unclear exactly when, the object is only ever defined on a global level). A warning of a memory leak occurs, and while references to the memory still appear valid, the file descriptor appears to no longer be accessible. It is noted online that the `multiprocessing.shared_memory` library may be at fault. Errors are avoided if a Python program is kept running as long as the shared memory exists. For this reason, the Python implementation defaults to `host = True`, as that appears to be its best use case;
- The byte array returned by `.getBuffer()` isn't the standard Python type and may not behave exactly the same. For best results, only treat the memory as a list of bytes and perform any appropriate casts manually.

## /across-devices/*

WIP
