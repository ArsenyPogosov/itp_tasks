#include "utf8.h"
#include <iostream>
#include <cstddef>
#include <vector>
#include <string>

void EncodeOne(uint32_t h, unsigned char* res, int& sz)
{
    if (0x00000000 <= h && h <= 0x0000007F) {
        res[0] = 0b00000000 | ((h>>0) & ((1 << 7) - 1));
        sz = 1;
    }
    if (0x00000080 <= h && h <= 0x000007FF) {
        res[1] = 0b10000000 | ((h>>0) & ((1 << 6) - 1));
        res[0] = 0b11000000 | ((h>>6) & ((1 << 5) - 1));
        sz = 2;
    }
    if (0x00000800 <= h && h <= 0x0000FFFF) {
        res[2] = 0b10000000 | ((h>>0) & ((1 << 6) - 1));
        res[1] = 0b10000000 | ((h>>6) & ((1 << 6) - 1));
        res[0] = 0b11100000 | ((h>>12) & ((1 << 4) - 1));
        sz = 3;
    }
    if (0x00010000 <= h && h <= 0x0010FFFF) {
        res[3] = 0b10000000 | ((h>>0) & ((1 << 6) - 1));
        res[2] = 0b10000000 | ((h>>6) & ((1 << 6) - 1));
        res[1] = 0b10000000 | ((h>>12) & ((1 << 6) - 1));
        res[0] = 0b11110000 | ((h>>18) & ((1 << 3) - 1));
        sz = 4;
    }
}

void ReadOne(unsigned char *from, uint32_t& res, int& len)
{
    if (((*from) & 0b10000000) == 0b00000000) {
        len = 1;
        res = ((uint32_t(from[0] & ((1 << 7) - 1))) << 0);
    }
    if (((*from) & 0b11100000) == 0b11000000) {
        len = 2;
        res = ((uint32_t(from[0] & ((1 << 5) - 1))) << 6) +
              ((uint32_t(from[1] & ((1 << 6) - 1))) << 0);
    }
    if (((*from) & 0b11110000) == 0b11100000) {
        len = 3;
        res = ((uint32_t(from[0] & ((1 << 4) - 1))) << 12) +
              ((uint32_t(from[1] & ((1 << 6) - 1))) << 6)  +
              ((uint32_t(from[2] & ((1 << 6) - 1))) << 0);
    }
    if (((*from) & 0b11111000) == 0b11110000) {
        len = 4;
        res = ((uint32_t(from[0] & ((1 << 3) - 1))) << 18) +
              ((uint32_t(from[1] & ((1 << 6) - 1))) << 12) +
              ((uint32_t(from[2] & ((1 << 6) - 1))) << 6)  +
              ((uint32_t(from[3] & ((1 << 6) - 1))) << 0);
    }

}

size_t CountUtf8Symbols(const std::string& str) {
    std::vector<uint32_t> res;
    DecodeUtf8(str, res);
    return res.size();
}

void EncodeUtf8(const std::vector<uint32_t>& codepoints, std::string& str) {
    int len = 0;
    unsigned char res[4*codepoints.size() * 4];
    for (auto&i: codepoints) {
        int tlen = 0;
        unsigned char tres[4];
        EncodeOne(i, tres, tlen);
        memcpy(res + len, tres, tlen);
        len += tlen;
    }

    str = *((std::string *)(res));
}

void DecodeUtf8(const std::string& str, std::vector<uint32_t>& codepoints) {
    codepoints.assign(0, 0);

    int len = 0;
    while (len < (int)str.size())
    {
        int tlen;
        uint32_t res;

        ReadOne((unsigned char*)(str.data()) + len, res, tlen);
        codepoints.push_back(res);
        len += tlen;
    }
}
