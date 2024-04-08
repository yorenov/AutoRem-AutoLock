#pragma once

#include "samp.h"
#include "CVector.h"

class CMatrix {
public:
    CVector       right;
    unsigned long flags;
    CVector       up;
    float         pad_u;
    CVector       at;
    float         pad_a;
    CVector       pos;
    float         pad_p;
};