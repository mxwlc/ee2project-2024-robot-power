#include "shared_mem.hpp"
#include "network_bg_proc_obj.hpp"
#include <chrono>
#include <thread>


int main() {

    // Connect to shared memory. Must be defined for duration of usage
    SharedMemory* shared_mem_obj = new SharedMemory(sizeof(NetworkSharedObj));
    // Get pointer to shared memory
    NetworkSharedObj* shared_mem = (NetworkSharedObj*) shared_mem_obj->ptr;

    // START MAIN LOOP

    // Can perform operations which do not read/write from shared memory.

    //Lock mutex
    shared_mem->mutex.lock();

    // Perform operations on memory. Buffer is shared_mem->data_cv, of size BUFFER_SIZE_CV
    for (int i = 0; i < BUFFER_SIZE_CV; ++i) {
        shared_mem->data_cv[i] = i;
    }

    // Mark data for sending
    shared_mem->send_flag_cv = true;
    // Unlock mutex
    shared_mem->mutex.unlock();

    // Can perform operations which do not read/write from shared memory.

    // Perform timingA logic with mutex unlocked
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // END MAIN LOOP

    return 0;
}