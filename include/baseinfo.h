#pragma once
#ifndef BASEINFO_H
#define BASEINFO_H

#ifdef _WIN32
#define  PATH_SEPARATOR "\\"
#define WIDE_PATH_SEPARATOR L"\\"
#else
#define  PATH_SEPARATOR "/"
#define WIDE_PATH_SEPARATOR L"/"
#endif

#endif