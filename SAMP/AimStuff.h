#pragma once

#include "samp.h"
#include "CVector.h"

namespace AimStuff {
    struct Aim {
        CVector front;
        CVector source;
        CVector sourceBeforeLookBehind;
        CVector up;
    };
} // namespace AimStuff
