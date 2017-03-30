/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

#include <iostream>

extern bool verbose;

#define LOG(msg) \
    do { \
        if (verbose) { \
            std::cout << msg; \
        } \
    } while(false)

#define FLAGLOG() \
    LOG(__FILE__ << ": " << __FUNCTION__ << " " << __LINE__ <<std::endl);
