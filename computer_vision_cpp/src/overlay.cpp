//
// Created by maxwe on 08/06/24.
//
#include "../include/overlay.hpp"

overlay::overlay() {
    window_height = WINDOW_HEIGHT;
    window_width = WINDOW_WIDTH;
}

overlay::~overlay() {}

bool overlay::within_bounds() {
    return false;
}

void overlay::draw() {}

