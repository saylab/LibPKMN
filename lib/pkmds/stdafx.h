#pragma once
#ifdef _MSC_VER
#define DllExport __declspec(dllexport)
#else
#define DllExport __attribute__((visibility("default")))
#endif
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>
#include <array>
#include <math.h>
#include <algorithm>
#include <ctime>
typedef unsigned char byte; // 8 bits
typedef unsigned short int uint16; // 16 bits
typedef unsigned int uint32; // 32 bits
#ifdef _MSC_VER
#define strcpy__ strcpy_s
#else
#define strcpy__ strcpy
#endif
static const int LANG_ID = 9;
static const int VERSION_GROUP = 11;
static const int GENERATION = 5;
static const int BUFF_SIZE = 955;
static const int NICKLENGTH = 11;
static const int OTLENGTH = 8;
static const int BOXNAMELENGTH = 20;

namespace pkmds
{
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);
    byte getbit(byte & input, byte n);
    byte getbit(uint16 & input, byte n);
    byte getbit(uint32 & input, byte n);
    void setbit(byte & input, byte n, byte val);
    void setbit(uint16 & input, byte n, byte val);
    void setbit(uint32 & input, byte n, byte val);
    void togglebit(byte & input, byte n);
    void togglebit(uint16 & input, byte n);
    void togglebit(uint32 & input, byte n);
}
