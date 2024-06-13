// NOTE: THIS IS NOT A PROGRAM, NOT TO BE INCLUDED USING #include OR INSERTED ALONGSIDE SOURCE CODE
// IT IS JUST INSTRUCTIONS ON HOW TO SETUP THE RECEIVER THREAD

// [1] Copy ardpiwire.cpp and ardpiwire.hpp to project directory
// [2] Add this to the beginning of the main file:
#include "ardpiwire.hpp"

// [3] Create an array with all variables you are interested in modifying (name is up to you)
float configurable_params[10];

// [4] Create references to these, using the same variable names as originally
#define VAR1_INDEX 0
#define VAR2_INDEX 1

float& var1 = configurable_params[VAR1_INDEX];
float& var2 = configurable_params[VAR2_INDEX];

// Note: these act identical to saying 'float var1; float var2;',
// ie var1 and var2 act like normal variables of type float, only they
// are allocated within an array.

// [5] Copy and paste the following function call into the setup() function:
BaseType_t cur_core = xPortGetCoreID(), targ_core;
if (cur_core == 0) targ_core = 1;
else targ_core = 0;
xTaskCreatePinnedToCore(ArdPiWire::receiverThreadMain, "Recv", 500, (void*) configurable_params, 1, &ArdPiWire::receiver_thread, targ_core);