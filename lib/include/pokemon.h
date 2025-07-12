#ifndef GBPOKEMON_H
#define GBPOKEMON_H

#include "typeDefs.h"

class Pokemon // The base Pokemon class
{
public:
    enum GetSet {
        
    }
private:
    bool getSetValue();
};

class GBPokemon : public Pokemon // The class for gen 1 and 2 Pokemon, since they share some conversion functions
{
public:
    virtual byte *getDataArray();
    virtual byte *getOTArray();
    virtual byte *getNicknameArray();
    virtual byte getExternalIndexNumber(); // The index value stored outside of the main data array, usually in the party
private:
    byte nicknameArray[11];
    byte OTArray[11];
    byte externalIndexNumber;
};

class Gen1Pokemon : public GBPokemon // The class for gen 1 Pokemon
{
public:
private:
    byte dataArray[33];
};

class Gen2Pokemon : public GBPokemon // The class for gen 2 Pokemon
{
public:
private:
    byte dataArray[31];
};

#endif