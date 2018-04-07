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

static struct EC ec;

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

u8 *x_to_u8_buffer(const s8 * hex){
    u32 len = strlen(hex);
    s8 xtmp[3] = {0, 0 ,0};
    u8 * res = (u8 * ) malloc(sizeof(u8) * len);
    u8 * ptr = res;
    if(len % 2 != 0)
        return __need_NULL;
    while(len--){
        xtmp[0] = * hex++;
        xtmp[1] = * hex++;
        *ptr++ = (u8) x_to_u64(xtmp);
    }
    return res;
}

void bn_print(const char * name, const u8 * a, const u32 n){
    printf("%s:\t", name);
    for(u32 i = 0; i < n; i++){
        printf("%02x", a[i]);
        printf("\n");
    }
}

int point_is_zero(const struct point * p){
    return isEltZero(p -> x) && isEltZero(p -> y);
}

void point_double(struct point * r) {
    if (isEltZero(r->y)) {
        bn_zero((u8 *) r, 40);
        return;
    }

    struct point
            pp = *r;

    u8 s[20], t[20],
            *px = pp.x, *py = pp.y,
            *rx = r->x, *ry = r->y;

    bn_mon_mul(t, px, px, EC.p, 20);
    bn_add(s, t, t, EC.p, 20);
    bn_add(s, s, t, EC.p, 20);
    bn_add(s, s, EC.a, EC.p, 20);
    bn_add(t, py, py, EC.p, 20);
    u8 u[20];
    bn_copy(u, t, 20);
    bn_mon_inv(t, u, EC.p, 20);
    bn_mon_mul(s, s, t, EC.p, 20);
    bn_mon_mul(rx, s, s, EC.p, 20);
    bn_add(t, px, px, EC.p, 20);
    bn_sub(rx, rx, t, EC.p, 20);
    bn_sub(t, px, rx, EC.p, 20);
    bn_mon_mul(ry, s, t, EC.p, 20);
    bn_sub(ry, ry, py, EC.p, 20);
}

void point_add(struct point * r, const struct point * q){
    if(point_is_zero(r)){
        *r = * q;
        return;
    }
    if(point_is_zero(q)){
        return;
    }
    struct point
            pp = * r,
            qq = * q;
    u8 = s[20], t[20], u[20], *px = pp.x, *pu = pp.y, *qx = qq.x, *qy = qq.y, *rx = r -> x, *ry = r -> y;

    bn_sub_1(u, qx, 20);
    if(isEltZero(u)){
        bn_sub_1(u, qy, py);
        if(isEltZero(u))
            point_double(r);
        else
            bn_zero((u8 *) r, 40);
        return;
    }
}

