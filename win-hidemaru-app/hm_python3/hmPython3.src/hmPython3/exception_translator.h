#pragma once

#include <windows.h>
#include <string>

extern std::wstring python_critical_exception_message();
LONG CALLBACK SWFilter(EXCEPTION_POINTERS *ExInfo);