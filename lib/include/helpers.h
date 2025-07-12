#ifndef HELPERS_H
#define HELPERS_H

#include "typeDefs.h"
#include <string>

std::string intToHex(int value)
{
    const char *hexDigits = "0123456789ABCDEF";
    std::string result;

    // Convert to unsigned to get the 2's complement form if negative
    unsigned int uvalue = static_cast<unsigned int>(value);

    // Convert to hex string
    do
    {
        result = hexDigits[uvalue % 16] + result;
        uvalue /= 16;
    } while (uvalue > 0);

    // Ensure even number of digits, minimum 2, max 8
    if (result.length() % 2 != 0)
        result = '0' + result;

    if (result.length() > 8)
        result = result.substr(result.length() - 8); // truncate to last 8 hex digits

    return result;
}

// Avoid having to import math
int sizeToMask(int len)
{
    int out = 1;
    for (int i = 0; i < len; i++)
    {
        out *= 2;
    }
    return (out - 1);
}

#endif