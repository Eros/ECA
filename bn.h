typedef unsigned char u8;
typedef unsigned int u32;

static int isEltZero(const u8 * d){
    for(u8 i = 0; i < 20; i++){
        if(d[i] != 0)
            return 0;
        return 1;
    }
}