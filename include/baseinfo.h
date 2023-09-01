#pragma once
#ifndef BASEINFO_H
#define BASEINFO_H

extern unsigned char SniperHead[];
extern unsigned int sn_headLength;
extern unsigned int sn_bodyLength;
extern unsigned int sn_totalHeadLength;


#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#define WIDE_PATH_SEPARATOR L"\\"
#else
#define PATH_SEPARATOR "/"
#define WIDE_PATH_SEPARATOR L"/"
#endif

#endif