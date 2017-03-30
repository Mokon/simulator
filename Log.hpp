/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

#include <iostream>

static bool verbose{false}; // TODO fix this

#define LOG(msg) \
    do { \
        if (verbose) { \
            std::cout << msg; \
        } \
    } while(false)
