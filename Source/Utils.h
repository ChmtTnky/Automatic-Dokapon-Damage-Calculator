#pragma once
#include <string>

template <typename T>
T HexToNum(char* buffer)
{
    char num[sizeof(T)];
    for (int i = sizeof(T) - 1; i >= 0; i--)
        num[i] = buffer[sizeof(T) - 1 - i];

    return *reinterpret_cast<T*>(num);
}

inline std::string HexToString(char* buffer)
{
    std::string output;
    for (int i = 0; buffer[i] != 0; i++)
        output += buffer[i];
    return output;
}