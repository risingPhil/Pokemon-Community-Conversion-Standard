#include "PokeBox.h"

PokeBox::PokeBox(PokemonTables *nTable)
{
    table = nTable;
}

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
        return boxStorage[index];
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

// This is used to load our data in from an array
void PokeBox::loadData(int generation, Language nLang, byte nDataArray[])
{
    if (nLang != ENGLISH)
    {
        return; // Other languages are not supported yet
    }
    for (int pkmnIndex = 0; pkmnIndex < nDataArray[0]; pkmnIndex++)
    {
        GBPokemon *newPkmn;

        if (generation == 1)
        {
            newPkmn = new Gen1Pokemon(table);
        }
        else if (generation == 2)
        {
            newPkmn = new Gen2Pokemon(table);
        }

        int externalIDOffset = 1;
        int dataOffset = externalIDOffset + (20 * 1) + 1;
        int trainerNameOffset = dataOffset + (20 * newPkmn->dataArraySize);
        int nicknameOffset = trainerNameOffset + (20 * newPkmn->OTArraySize);

        externalIDOffset += pkmnIndex * 1;
        dataOffset += pkmnIndex * newPkmn->dataArraySize;
        trainerNameOffset += pkmnIndex * newPkmn->OTArraySize;
        nicknameOffset += pkmnIndex * newPkmn->nicknameArraySize;

        newPkmn->loadData(
            nLang,
            &nDataArray[dataOffset],        // Pokemon Data
            &nDataArray[nicknameOffset],    // Nickname
            &nDataArray[trainerNameOffset], // Trainer Name
            nDataArray[externalIDOffset]    // External ID Number
        );

        addPokemon(newPkmn);
    }
}

void PokeBox::convertPkmn(int index, bool sanitizeMythicals)
{
    Gen3Pokemon *convertedPkmn = new Gen3Pokemon(table);
    Pokemon *basePkmn = getPokemon(index);
    GBPokemon *oldPkmn = dynamic_cast<GBPokemon *>(basePkmn);

    oldPkmn->convertToGen3(convertedPkmn, sanitizeMythicals);
    delete boxStorage[index];
    boxStorage[index] = convertedPkmn;
}

void PokeBox::convertAll(bool sanitizeMythicals)
{
    for (int i = 0; i < currIndex; i++)
    {
        convertPkmn(i, sanitizeMythicals);
    }
}
#if INCLUDE_IOSTREAM
std::string PokeBox::printDataArray()
{
    std::stringstream ss;
    for (int i = 0; i < currIndex; i++)
    {
        if (boxStorage[i]->generation == 3)
        {
            ss << dynamic_cast<Gen3Pokemon *>(boxStorage[i])->printDataArray(true) << "\n";
        }
    }
    return ss.str();
}
#endif