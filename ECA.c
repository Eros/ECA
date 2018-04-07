#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bn.h"

struct point {
    u8 x[20];
    u8 y[20];
};

struct eca {
    u8 p[20];
    u8 a[20];
    u8 b[20];
    struct point G;
};

static struct ECA ec;

typedef char s8;
typedef unsigned long long int u64;

u64 x_to_u64(const s8 * hex){
    u64 t = 0;
    u64 res = 0;
    u32 len = strlen(hex);
    char c;
    while(len--){
        c = * hex++;
        if(c >= '0' && c <= '9')
            t = c - '0';
        else if(c >= 'a' && c <= 'f')
            t = c - 'a' + 10;
        else if(c >= 'A' && c <= 'F')
            t = c - 'A' + 10;
        else
            t = 0;
            res |= t << (len * 4);
    }
    return res;
}

