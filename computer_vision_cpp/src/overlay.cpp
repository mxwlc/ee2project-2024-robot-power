//
// Created by maxwe on 08/06/24.
//
#include "../include/overlay.hpp"

overlay::overlay() {
    *corners = {};
    height = 0;
    length = 0;
}

overlay::~overlay() {
    delete corners;
}
