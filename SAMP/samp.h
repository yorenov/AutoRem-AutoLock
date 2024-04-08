#pragma once
#include <Windows.h>

typedef void(__cdecl* CMDPROC)(const char*);
typedef unsigned long  TICK;
typedef int            GTAREF;
typedef int            BOOL;
typedef unsigned long D3DCOLOR;
typedef unsigned char  NUMBER;
typedef unsigned short ID;

unsigned long GetBase();

inline unsigned long GetAddress(signed long offset) {
    return GetBase() + offset;
}