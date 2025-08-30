#ifndef POKEBOX_H
#define POKEBOX_H

#include "Gen1Pokemon.h"
#include "Gen2Pokemon.h"
#include "Gen3Pokemon.h"

class PokeBox // Stores up to 30 Pokemon in a box
{
private:
    Pokemon *boxStorage[30];
    int currIndex = 0;

public:
    bool addPokemon(Pokemon *currPkmn);
    Pokemon *getPokemon(int index);
    bool removePokemon(int index);

#if INCLUDE_IOSTREAM
    friend std::ostream &operator<<(std::ostream &os, PokeBox &pc)
    {
        for (int i = 0; i < pc.currIndex; i++)
        {
            os << "\n" << 
            "---------------- " << "POKEMON #" << i << " ----------------" << "\n" <<
            *pc.boxStorage[i] << "\n";
        }

        return os;
    }
#endif
};

#endif