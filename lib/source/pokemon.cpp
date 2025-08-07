#include "pokemon.h"

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

u32 Pokemon::getVar(DataVarInfo dataVar)
{
    u32 out = 0;
    if (dataVar.dataLength < 8)
    { // is less than a byte, do bitwise stuff on a single byte
        out = (dataArrayPtr[dataVar.byteOffset] >> dataVar.bitOffset) & sizeToMask(dataVar.dataLength);
    }
    else
    { // is larger than a byte, will have to access multiple parts of the array
        int num_bytes = dataVar.dataLength / 8;
        for (int i = 0; i < num_bytes; i++)
        {
            out = (out << 8) | dataArrayPtr[dataVar.byteOffset + i];
        }
    }
    return out;
}

bool Pokemon::setVar(DataVarInfo dataVar, int newValue)
{
    if (dataVar.dataLength < 8)
    { // is less than a byte, do bitwise stuff on a single byte
        dataArrayPtr[dataVar.byteOffset] &= ~(sizeToMask(dataVar.dataLength) << dataVar.bitOffset);
        dataArrayPtr[dataVar.byteOffset] |= (newValue & sizeToMask(dataVar.dataLength)) << dataVar.bitOffset;
    }
    else
    { // is larger than a byte, will have to access multiple parts of the array
        int numBytes = dataVar.dataLength / 8;
        for (int i = 0; i < numBytes; i++)
        {
            dataArrayPtr[dataVar.byteOffset + i] = (newValue >> (8 * ((numBytes - 1) - i)));
        }
    }
    return true;
}

GBPokemon::GBPokemon()
{
    nicknameArrayPtr = nicknameArray;
    OTArrayPtr = OTArray;
    externalIndexNumberPtr = &externalIndexNumber;
}