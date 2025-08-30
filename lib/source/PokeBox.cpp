#include "PokeBox.h"

bool PokeBox::addPokemon(Pokemon *currPkmn)
{
    if (currIndex < 30)
    {
        boxStorage[currIndex] = currPkmn;
        currIndex++;
        return true;
    }
    return false;
}

Pokemon *PokeBox::getPokemon(int index)
{
    if (index < currIndex)
    {
        return boxStorage[currIndex];
    }
    return nullptr;
}

bool PokeBox::removePokemon(int index)
{
    if (index < currIndex)
    {
        for (int i = index; i < currIndex; i++)
        {
            boxStorage[i] = boxStorage[i + 1];
        }
        return true;
    }
    return false;
}