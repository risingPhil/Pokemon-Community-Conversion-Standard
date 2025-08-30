#ifndef GEN1POKEMON_H
#define GEN1POKEMON_H

#include "GBPokemon.h"

class Gen1Pokemon : public GBPokemon // The class for gen 1 Pokemon
{
public:
    Gen1Pokemon();

    byte dataArray[33];

    u32 getCurrentHP() { return getVar(g1_currentHP); }
    u32 getStatusCondition() { return getVar(g1_statusCondition); }
    u32 getCatchRate() { return getVar(g1_catchRate); }

    bool setCurrentHP(byte newVal) { return setVar(g1_currentHP, newVal); }
    bool setStatusCondition(byte newVal) { return setVar(g1_statusCondition, newVal); }
    bool setCatchRate(byte newVal) { return setVar(g1_catchRate, newVal); }

    u32 getType(int typeIndex) { return getVar(*g1_types[typeIndex]); }
    bool setType(int typeIndex, Gen1Types newVal) { return setVar(*g1_types[typeIndex], newVal); }

#if INCLUDE_IOSTREAM
    void print(std::ostream &os);
#endif

    bool setNewSpeciesIndex(Gen3Pokemon *newPkmn);
};

#endif