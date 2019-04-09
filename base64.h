/**
 * 
 * @author:       kylin
 * @email:        kylin.du@outlook.com
 * @dateTime:     2018-06-06 Wed 15:34:58
 * @copyright:    kylin
 * @description:  
 * 
 */
#ifndef _BASE64_H_
#define _BASE64_H_

namespace NetCore {
const unsigned char base64_str_tbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


class base64 {
public:
    inline static int encode(void* dst, unsigned int dlen, const void* src, unsigned int slen) {
        unsigned int left = slen % 3;
        unsigned int cn = slen/3;
        
        if (dlen < ((cn << 2) + (left > 0 ? 4 : 0))) {
            return -1;
        }

        unsigned char* pdst = (unsigned char*)dst;
        const unsigned char* psrc = (const unsigned char*)src;
        unsigned int di = 0;

        unsigned int sval = slen - left;
        for (unsigned int i = 0; i < sval; i += 3) {
            pdst[di++] = base64_str_tbl[((psrc[i]) >> 2)];
            pdst[di++] = base64_str_tbl[(((psrc[i]) & 0x03) << 4) | ((psrc[i+1]) >> 4)];
            pdst[di++] = base64_str_tbl[(((psrc[i+1]) & 0x0f) << 2) | ((psrc[i+2]) >> 6)];
            pdst[di++] = base64_str_tbl[((psrc[i+2]) & 0x3f)]; 
        }
        
        if (left == 2) {
            pdst[di++] = base64_str_tbl[((psrc[sval]) >> 2)];
            pdst[di++] = base64_str_tbl[(((psrc[sval]) & 0x03) << 4) | ((psrc[sval+1]) >> 4)];
            pdst[di++] = base64_str_tbl[(((psrc[sval+1]) & 0x0f) << 2)];
            //pdst[di++] = '=';
        }else if (left == 1){
            pdst[di++] = base64_str_tbl[((psrc[sval]) >> 2)];
            pdst[di++] = base64_str_tbl[(((psrc[sval]) & 0x03) << 4)];
            //pdst[di++] = '=';
            //pdst[di++] = '=';
        }

        return di;
    }

    inline static unsigned char base64_tbl_val(unsigned char val) {
        if (val >= 'A' && val <= 'Z') {
            return (val - 'A');
        } else if (val >= 'a' && val <= 'z') {
            return ((val - 'a') + 26);
        } else if (val >= '0' && val <= '9') {
            return ((val - '0') + 52);
        } else if (val == '+') {
            return 62;
        } else if (val == '/') {
            return 63;
        }
        return 0xff;
    }

    inline static int decode(void* dst, unsigned int dlen, const void* src, unsigned int slen) {
        unsigned char* pdst = (unsigned char*)dst;
        const unsigned char* psrc = (const unsigned char*)src;
        unsigned char v[4];
        unsigned int di = 0;

        if (slen < 2) {
            return -1;
        }
        //adapter to fix = format
        if (psrc[slen - 1] == '=') {
            if (slen & 3 != 0) {
                return -1;
            }
            if (psrc[slen - 2] == '=') {
                slen -= 2;
            }else {
                slen -= 1;
            }
        }

        unsigned int left = slen & 3;
        unsigned int sval = slen - left;

        
        for (unsigned i = 0; i < sval; i += 4) {
            if ((v[0] = base64_tbl_val(psrc[i])) > 0x3f
                || (v[1] = base64_tbl_val(psrc[i+1])) > 0x3f
                || (v[2] = base64_tbl_val(psrc[i+2])) > 0x3f
                || (v[3] = base64_tbl_val(psrc[i+3])) > 0x3f) {
                    return -1;
            }
            pdst[di++] = (v[0] << 2) | ((v[1] & 0x30) >> 4);
            pdst[di++] = ((v[1] & 0x0f) << 4) | (v[2] >> 2);
            pdst[di++] = (v[2] & 0x03) << 6 | v[3];
        }

        if (left == 3) {
            if ((v[0] = base64_tbl_val(psrc[sval])) > 0x3f
                || (v[1] = base64_tbl_val(psrc[sval+1])) > 0x3f
                || (v[2] = base64_tbl_val(psrc[sval+2])) > 0x3f) {
                return -1;
            }
            pdst[di++] = (v[0] << 2) | ((v[1] & 0x30) >> 4);
            pdst[di++] = ((v[1] & 0x0f) << 4) | (v[2] >> 2);

        } else if (left == 2) {
            if ((v[0] = base64_tbl_val(psrc[sval])) > 0x3f
                || (v[1] = base64_tbl_val(psrc[sval+1])) > 0x3f) {
                return -1;
            }
            pdst[di++] = (v[0] << 2) | ((v[1] & 0x30) >> 4);
        } else if (left == 1) {
            return -1;
        }

        return di;
    }
};


};


#endif //_BASE64_H_