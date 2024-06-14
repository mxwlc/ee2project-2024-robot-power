
# Communications

  

## Overview

  

This branch handles the various necessary forms of communication throughout the project. These are:

  

- Shared memory across processes (C++);

- Wired transmission to/from Raspberry Pi from/to Arduino;

- Wireless sending to/from Raspberry Pi from/to Laptop [Pi - COMPLETE, LAPTOP - INCOMPLETE, WIP].

  

## /within-rpi/\* [EXCEPT shell/]

  

This is an implementation of a `SharedMemory` class which creates/connects to a shared memory instance with a specified name and size in bytes. Intended for use on the Raspberry Pi to allow for communication between processes.

  

### Instructions

  

#### C++:

  

Create an instance of the `SharedMemory` class, then the `ptr` member can be used to access the memory (note: `ptr` is left of type `void *` and can be cast as is most convenient).


Notes:

- Intended for static implementation: copy the `.cpp` and `.hpp` into the same directory as your source code, include the `.hpp` in your code and add the `.cpp` as a source file when compiling;

  

#### Python (NOT FULLY TESTED, DEEMED UNNECESSARY, ONLY LEFT AS A JUST-IN-CASE):

  

Import the `.py` file and create an instance of the `SharedMemory` class. Then use the `.getBuffer()` method to get the shared memory as a byte array.

 

Notes:

- Unintended behaviour occurs when a shared memory object, which connected to an existing shared memory object created either in Python or C++, is destroyed or the program terminates (unclear exactly when, the object is only ever defined on a global level). A warning of a memory leak occurs, and while references to the memory still appear valid, the file descriptor appears to no longer be accessible. It is noted online that the `multiprocessing.shared_memory` library may be at fault. Errors are avoided if a Python program is kept running as long as the shared memory exists. For this reason, the Python implementation defaults to `host = True`, as that appears to be its best use case;

- The byte array returned by `.getBuffer()` isn't the standard Python type and may not behave exactly the same. For best results, only treat the memory as a list of bytes and perform any appropriate casts manually.


## /within-rpi/shell/\*

This contains shell scripts used to make the Raspberry Pi act as a hotspot on startup (SSID: Robot_Power_Bot, password: RobotPower123) and enable ssh. It is largely meant as a reference point, as usually these would have to be manually retyped on the Pi. 


## /across-devices/laptop/\*, /across-devices/rpi/\* [EXCEPT ardpiwire.\*]

This implements a TCP socket connection between the Raspberry Pi and Laptop. The Raspberry Pi networking code is then used to make a background process which sends data queued into the shared memory, which is enabled at the Pi's startup alongside its hotspot.

For instructions on sending from the Pi to the laptop, see the `example_cv_write.cpp` file which writes the proposed buffer written to by OpenCV to the shared memory, and a compiled instance of `network_bg_proc.cpp` then sends it across sockets.

Note: the objects being sent are in `network_bg_proc_obj.hpp`. Let me know if you need something sent from the Pi, I'll help with adding it and updating the other files appropriately.

#### Instructions for using the laptop code:

- Include the `network_proc.py` file and import it in your code;
- Follow the instructions in `network_proc.py`'s main body (after `if __name__ == "__main__"`).


## /across-devices/rpi/ardpiwire.\*, /across-devices/arduino/\*

This contains functions in an `ArdPiWire` namespace, with an `init()` function which needs calling at the start of either devices' code, then `sendByte()`, `sendFloat()`, `recvByte()` and `recvFloat()` for both devices. Some data can be seen on the speeds observed with different delays, and any diagrams in the writeup will also be provided.

Note: the sending and receiving isn't meant to be "general purpose". It is designed to work as follows:

- The Pi requests wanting to send data;
- The Arduino responds with an acknowledgement;
- The Pi now outputs a clock during which data is sent (these last 3 steps are heavily inspired by SPI);
- Once a full byte is sent, the Arduino treats this as an instruction. Both the Pi and Arduino know how many successive sends/receives are needed (this means the Arduino only sends in response to a receive, it does not need the same request/acknowledge that we have from Pi to Arduino).

Note: we merged the Arduino's send pin and acknowledge pin into being the same, and its behaviour changes based on whether a send or receive is occurring. This narrowed the transmission to requiring 3 pins/wires.
