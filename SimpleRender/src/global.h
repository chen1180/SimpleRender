#pragma once
//#include qt
#include<qopenglshaderprogram.h>
#include<qopenglfunctions_4_3_core.h>
#include<qopenglframebufferobject.h>
#include<qopenglbuffer.h>
#include<qmatrix4x4.h>
#include<gl/GL.h>
#include<memory>

//using name
using std::make_shared;
using std::make_unique;
using std::unique_ptr;
using std::shared_ptr;

//common function
static int nextPowerOfTwo(int x) {
    x--;
    x |= x >> 1; // handle 2 bit numbers
    x |= x >> 2; // handle 4 bit numbers
    x |= x >> 4; // handle 8 bit numbers
    x |= x >> 8; // handle 16 bit numbers
    x |= x >> 16; // handle 32 bit numbers
    x++;
    return x;
}