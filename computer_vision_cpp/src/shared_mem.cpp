#include "shared_mem.hpp"

SharedMemory::SharedMemory(const char* name, const int size, const bool host)
: name(name), size(size), host(host) {
    if (host) {
        fd = shm_open(name, O_CREAT | O_RDWR, 666);
        if (fd == -1) {
            perror("shm_open");
            return;
        }
        ftruncate(fd, size);
        ptr = mmap(nullptr, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        if (ptr == nullptr) perror("mmap");
    }
    else {
        fd = shm_open(name, O_RDWR, 666);
        ptr = mmap(nullptr, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        if (ptr == nullptr) perror("mmap");
    }
}

SharedMemory::SharedMemory(const int size, const bool host)
: SharedMemory(SHARED_MEM_NAME, size, host) { }

SharedMemory::~SharedMemory() {
    if (host) {
        munmap(ptr, size);
        shm_unlink(name);
    }
}