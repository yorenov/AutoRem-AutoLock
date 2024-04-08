#include "samp.h"
#include <windows.h>

unsigned long GetBase() {
    static auto handle = reinterpret_cast<unsigned long>(GetModuleHandleA("samp.dll"));
    return handle;
}