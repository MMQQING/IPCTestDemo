#pragma once
#include "include/NetUnitInterface.h"

#ifdef _DEBUG
#pragma comment(lib, "./lib/debug/libprotobufd.lib")
#pragma comment(lib, "./lib/debug/NetUnitBoostx64d.lib")
#else
#pragma comment(lib, "./lib/release/libprotobufd.lib")
#pragma comment(lib, "./lib/release/NetUnitBoostx64.lib")
#endif