#pragma once

#include <mutex>

// Set the size of the buffers here
#define BUFFER_SIZE_CV 640 * 480 * 3

struct NetworkSharedObj {

    std::mutex mutex;

    bool send_flag_cv;
    char data_cv[BUFFER_SIZE_CV];
    char dir_cv;
    int targ_id_cv;


    bool send_flag_var;
    char index_var;
    float data_var;

};

// 1259