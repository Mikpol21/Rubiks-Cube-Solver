#pragma once

#ifdef DEBUG_BUILD
#include <iostream>
#define DEBUG(x) std::cout << x << std::endl
#else
#define DEBUG(x)
#endif

#ifndef SOURCE_DIR
#define SOURCE_DIR "."
#endif