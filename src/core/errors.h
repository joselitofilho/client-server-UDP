#pragma once

#include <iostream>

void error(const char *msg)
{
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}
