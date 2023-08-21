#include "baseinfo.h"

unsigned char SniperHead[] =
{
    0xFE, 0xFE, 0xFE, 0xFE, //标识头
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, //数据长度(大端序)
    0x01,                   //压缩类型
};

unsigned int sn_headLength = 4;
unsigned int sn_bodyLength = 8;
unsigned int sn_totalHeadLength = 13;
