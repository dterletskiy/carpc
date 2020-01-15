#pragma once

#include <type_traits>



#define COMPARE_TYPES( TYPE_A, TYPE_B ) std::is_same_v< TYPE_A, TYPE_B >

#define DEAD *((unsigned int*)0) = 0xDEAD;
