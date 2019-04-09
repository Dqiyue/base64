#include "../base64.h"
#include <stdio.h>

int main() {
    char dst[1024];
    char dstr[1024];
    char data[] = "it is a test base64 example! >> hahahahahahahaha!";
    printf("origin data : %s\n", data);
    int n = NetCore::base64::encode(dst, 1024, data, sizeof data);
    if (n < 0) {
        printf("base64encode error!\n");
        return 0;
    }
    dst[n] = 0;
    printf("encode: %s\n",dst);
    int nd = NetCore::base64::decode(dstr,1024,dst,n);
    if (nd < 0) {
        printf("base64decode error!\n");
        return 0;
    }
    dstr[nd] = 0;
    printf("decode: %s\n",dstr);
    return 0;
}

