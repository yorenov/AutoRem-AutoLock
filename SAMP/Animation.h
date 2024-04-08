#pragma once

#include "samp.h"

struct Animation {
    union {
        struct {
            unsigned short m_nId : 16;
            unsigned char  m_nFramedelta : 8;
            unsigned char  m_nLoopA : 1;
            unsigned char  m_nLockX : 1;
            unsigned char  m_nLockY : 1;
            unsigned char  m_nLockF : 1;
            unsigned char  m_nTime : 2;
        };
        int m_value;
    };
};