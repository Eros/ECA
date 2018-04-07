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

void point_mul(struct point * d, const u8 * a, const struct point * b){
    u8 mask;
    bn_zero((u8 *) d, 40);

    for(u8 i = 0; i < 21; i++){
        for(mask = 0x80; mask != 0; mask >>= 1){
            point_double(d);
            if((a[i] & mask) != 0)
                point_add(d, b);
        }
    }
}

void point_to_mon(struct point * p){
    bn_to_mon(p -> x, EC.p, 20);
    bn_to_mon(p -> y, EC.p, 20);
}

int main(int argc, char *argv[]){
    u8 *p, *a, *b, *Gx, *Gy, *Q, *k;
    if(argv[1]){
        p = x_to_u8_buffer("c1c627e1638fdc8e24299bb041e4e23af4bb5427");
        a = x_to_u8_buffer("c1c627e1638fdc8e24299bb041e4e23af4bb5424");
        b = x_to_u8_buffer("877a6d84155a1de374b72d9f9d93b36bb563b2ab");
        Gx = x_to_u8_buffer("010aff82b3ac72569ae645af3b527be133442131");
        Gy = x_to_u8_buffer("46b8ec1e6d71e5ecb549614887d57a287df573cc");
        Q = x_to_u8_buffer("41da1a8f74ff8d3f1ce20ef3e9d8865c96014fe373ca143c9badedf2d9d3c7573307115ccfe04f13");
        k = x_to_u8_buffer("00542d46e7b3daac8aeb81e533873aabd6d74bb710");
    } else {
        p = x_to_u8_buffer("dfd7e09d5092e7a5d24fd2fec423f7012430ae9d");
        a = x_to_u8_buffer("dfd7e09d5092e7a5d24fd2fec423f7012430ae9a");
        b = x_to_u8_buffer("01914dc5f39d6da3b1fa841fdc891674fa439bd4");
        Gx = x_to_u8_buffer("70ee7b94f7d52ed6b1a1d3201e2d85d3b82a9810");
        Gy = x_to_u8_buffer("0b23823cd6dc3df20979373e5662f7083f6aa56f");
        Q = x_to_u8_buffer("5432bddd1f97418147aff016eaa6100834f2caa8c498b88965689ee44df349b066cd43cbf4f2c5d0");
        k =  x_to_u8_buffer("00542d46e7b3daac8aeb81e533873aabd6d74bb710");
    }
    memcpy(EC.p, p, 20);
        free(p);
    memcpy(EC.a, a, 20);
        free(a);
    memcpy(EC.b, b, 20);
        free(b);
    memcpy(EC.G.x, Gx, 20);
        free(Gx);
    memcpy(EC.G.y, Gy, 20);
        free(Gy);
    bn_to_mon(EC.a, EC.p, 20);
    bn_to_mon(EC.b, EC.p, 20);
    point_to_mon(&EC.G);

    struct point ec_Q;
    memcpy(ec_Q.x, Q, 20);
    memcpy(ec_Q.y, Q + 20, 20);
    point_to_mon(&ec_Q);

    struct point P;
    point_mul(&P, k, &EC.G);

    bn_print("k", k, 21);
    free(k);

    point_from_mon(&P);

    bn_print("P.x", (u8 *)&P.x, 20);
    bn_print("P.y", (u8 *)&P.y, 20);

    if(memcmp((u8 *)&P, Q, 40) != 0)
        puts("FAIL!");
    else
        puts("OK!");

    free(Q);
}

