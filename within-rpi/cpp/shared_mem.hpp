#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>

#pragma once

#define SHARED_MEM_NAME "/shared_mem"

class SharedMemory {
    const char* name;
    const int size;
    const bool host;
    int fd;

public:
    void* ptr;

    SharedMemory(const char* name, const int size, const bool host = false);
    SharedMemory(const int size, const bool host = false);
    ~SharedMemory();
};