#include "Gen3Pokemon.h"

Gen3Pokemon::Gen3Pokemon(PokemonTables *table)
{
    pokeTable = table;
    dataArrayPtr = dataArray;
    dataArraySize = 80;
    isBigEndian = false;
    isEncrypted = false;
    generation = 3;
};

bool Gen3Pokemon::convertToGen3(Gen3Pokemon *g3p)
{
    return false;
}

// This is used to easily print out a Pokemon, when using a standard C++ terminal
#if INCLUDE_IOSTREAM
void Gen3Pokemon::print(std::ostream &os)
{
    updateChecksum();
    updateSubstructureShift();

    pokeTable->load_gen3_charset(ENGLISH);
    if (isInvalid)
    {
        os << "ERROR: POKEMON IS INVALID\n";
    }
    else
    {
        os
            << "Personality Value: " << std::hex << getPersonalityValue() << std::dec
            << "\n\tLetter: " << (int)getUnownLetter()
            << "\n\tNature: " << getNature()
            << "\n\tGender: " << getGender() << '\n'
            << "Trainer ID: " << getTrainerID() << "\n"
            << "Secret ID: " << getSecretID() << "\n"
            << "Nickname: [";

        for (int i = 0; i < 10; i++)
        {
            os << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)getNicknameLetter(i) << (i < 9 ? ", " : "");
        }

        os << "] (";

        for (int i = 0; i < 10; i++)
        {
            os << (char)(pokeTable->gen3_charset[(int)getNicknameLetter(i)]);
        }

        os << ")" << "\n"
           << "Original Trainer: [";

        for (int i = 0; i < 7; i++)
        {
            os << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)getOTLetter(i) << (i < 6 ? ", " : "");
        }

        os << "] (";

        for (int i = 0; i < 7; i++)
        {
            os << (char)(pokeTable->gen3_charset[(int)getOTLetter(i)]);
        }

        os << ")" << "\n"
           << std::dec
           << "Language: " << getLanguage() << "\n"
           << "Is Bad Egg: " << getIsBadEgg() << "\n"
           << "Has Species: " << getHasSpecies() << "\n"
           << "Use Egg Name: " << getUseEggName() << "\n"
           << "Block Box RS: " << getBlockBoxRS() << "\n"
           << "Markings: " << getMarkings() << "\n"
           << "Checksum: " << getChecksum() << " (is valid = " << true << ")\n"
           << "Species Index Number: " << getSpeciesIndexNumber() << "\n"
           << "Held Item: " << getHeldItem() << "\n"
           << "Experience: " << getExpPoints() << "\n"
           << "Friendship: " << getFriendship() << "\n"
           << "Stats: "
           << "\n\tHP: " << getEV(HP) << " EVs, " << getIV(HP) << " IVs"
           << "\n\tAttack: " << getEV(ATTACK) << " EVs, " << getIV(ATTACK) << " IVs"
           << "\n\tDefense: " << getEV(DEFENSE) << " EVs, " << getIV(DEFENSE) << " IVs"
           << "\n\tSpecial Attack: " << getEV(SPECIAL_ATTACK) << " EVs, " << getIV(SPECIAL_ATTACK) << " IVs"
           << "\n\tSpecial Defense: " << getEV(SPECIAL_DEFENSE) << " EVs, " << getIV(SPECIAL_DEFENSE) << " IVs"
           << "\n\tSpeed: " << getEV(SPEED) << " EVs, " << getIV(SPEED) << " IVs" << "\n"
           << "Contest Stats: "
           << "\n\tCoolness: " << getContestCondition(COOLNESS)
           << "\n\tBeauty: " << getContestCondition(BEAUTY)
           << "\n\tCuteness: " << getContestCondition(CUTENESS)
           << "\n\tSmartness: " << getContestCondition(SMARTNESS)
           << "\n\tToughness: " << getContestCondition(TOUGHNESS)
           << "\n\tSheen: " << getSheen() << "\n"
           << "Moves: "
           << "\n\t" << getMove(0) << " (" << getPPTotal(0) << " PP, " << getPPUpNum(0) << " PP Ups" << ")"
           << "\n\t" << getMove(1) << " (" << getPPTotal(1) << " PP, " << getPPUpNum(1) << " PP Ups" << ")"
           << "\n\t" << getMove(2) << " (" << getPPTotal(2) << " PP, " << getPPUpNum(2) << " PP Ups" << ")"
           << "\n\t" << getMove(3) << " (" << getPPTotal(3) << " PP, " << getPPUpNum(3) << " PP Ups" << ")" << "\n"
           << "Pokerus: "
           << "\n\tStrain: " << getPokerusStrain()
           << "\n\tDays Remaining: " << getPokerusDaysRemaining() << "\n"
           << "Met Location: " << getMetLocation() << "\n"
           << "Level Met: " << getLevelMet() << "\n"
           << "Game of Origin: " << getGameOfOrigin() << "\n"
           << "Pokeball Caught In: " << getPokeballCaughtIn() << "\n"
           << "Original Trainer Gender: " << getOriginalTrainerGender() << "\n"
           << "Is Egg: " << getIsEgg() << "\n"
           << "Ability: " << getAbility() << "\n"
           << "Fateful Encounter/Obedience: " << getFatefulEncounterObedience() << "\n"
           << "Is Shiny: " << getIsShiny() << "\n"
           << "\n"
           << "Substructure Perm: " << currSubstructureShift << "\n"
           << "Encryption Key: " << std::hex << ((getTrainerID() | getSecretID() << 16) ^ getPersonalityValue()) << std::dec << "\n"
           << "Substructure offsets:"
           << "\n\tG: " << substructOffsets[SUB_G]
           << "\n\tA: " << substructOffsets[SUB_A]
           << "\n\tE: " << substructOffsets[SUB_E]
           << "\n\tM: " << substructOffsets[SUB_M]
           << "\n";
    }
};
std::string Gen3Pokemon::printDataArray(bool encrypedData)
{
    updateSubstructureShift();
    updateChecksum();
    encryptSubstructures();
    std::stringstream ss;
    for (int i = 0; i < 80; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)dataArray[i] << (i < 79 ? " " : "");
    }
    return ss.str();
}
#endif

// Get Nickname is different
// u32 getNickname() {};
// u32 getOriginalTrainerName() {}

u32 Gen3Pokemon::getNextRand_u32()
{
    currRand = (0x41C64E6D * currRand) + 0x6073;
    // std::cout << "Rolled: " << std::hex << currRand << "\n";
    return currRand;
}

u32 Gen3Pokemon::getPrevRand_u32()
{
    currRand = (currRand - 24691) * 4005161829;
    return currRand;
}

u16 Gen3Pokemon::getNextRand_u16()
{
    return getNextRand_u32() >> 16;
}

u16 Gen3Pokemon::getPrevRand_u16()
{
    return getPrevRand_u32() >> 16;
}

bool Gen3Pokemon::setPersonalityValue(u32 newVal) // Setting the Personality Value should modify some other values as well
{
    bool successful = setVar(personalityValue, newVal);
    if (successful)
    {
        successful &= setAbility(getPersonalityValue() & 0b1);
    }
    return successful;
}

bool Gen3Pokemon::setAbility(u32 newVal) // We need to check if they have two abilities
{
    if (pokeTable->get_num_abilities(getSpeciesIndexNumber()) == 0)
    {
        newVal = 0;
    }
    return setVar(ability, substructOffsets[SUB_M], newVal);
}

// This is used to load our data in from an array and mark it as encrypted
void Gen3Pokemon::loadData(byte incomingArray[])
{
    for (int i = 0; i < dataArraySize; i++)
    {
        dataArrayPtr[i] = incomingArray[i];
    }
    isEncrypted = true;
}

// And then some general functions
void Gen3Pokemon::decryptSubstructures()
{
    if (isEncrypted)
    {
        u32 key = (getTrainerID() | getSecretID() << 16) ^ getPersonalityValue();
        for (int i = 0; i < 48; i++)
        {
            dataArrayPtr[0x20 + i] ^= ((key >> (8 * (i % 4))) & 0xFF);
        }
    }
};

void Gen3Pokemon::encryptSubstructures()
{
    if (!isEncrypted)
    {
        u32 key = (getTrainerID() | getSecretID() << 16) ^ getPersonalityValue();
        for (int i = 0; i < 48; i++)
        {
            dataArrayPtr[0x20 + i] ^= ((key >> (8 * (i % 4))) & 0xFF);
        }
    }
};

void Gen3Pokemon::updateChecksum()
{
    bool encryptionState = isEncrypted;
    decryptSubstructures();
    int checksum = 0x0000;
    for (int i = 0; i < 48; i = i + 2)
    {
        checksum = checksum + ((dataArrayPtr[0x20 + i + 1] << 8) | dataArrayPtr[0x20 + i]);
    }
    setChecksum(checksum);
    if (encryptionState)
    {
        encryptSubstructures();
    }
}

void Gen3Pokemon::updateSubstructureShift()
{
    int structureVal = getPersonalityValue() % 24;
    if (structureVal == currSubstructureShift)
    {
        return;
    }
    currSubstructureShift = structureVal;

    resetSubstructureShift();

#define MAX_LEN 4
    int index = 0;
    while (index < MAX_LEN)
    {
        int len = MAX_LEN - index;
        int factorial = 1;
        for (int i = 1; i < len; i++)
        {
            factorial *= i;
        }
        int swapLoc = (structureVal / factorial) + index;
        for (int i = index; i < swapLoc; i++)
        {
            swapSubstructures(index, (i + 1));
        }
        index += 1;
        structureVal %= factorial;
    }
}

void Gen3Pokemon::resetSubstructureShift()
{
    for (int currDest = 0; currDest < 4; currDest++)
    {
        for (int i = 0; i < 4; i++)
        {
            if ((substructOffsets[i] / 12) == currDest)
            {
                swapSubstructures(currDest, i);
            }
        }
    }
}

void Gen3Pokemon::swapSubstructures(int indexOne, int indexTwo)
{
    if (indexOne == indexTwo)
    {
        return;
    }

    byte tempByte;
    for (int i = 0; i < 12; i++)
    {
        tempByte = dataArrayPtr[0x20 + (indexOne * 12) + i];
        dataArrayPtr[0x20 + (indexOne * 12) + i] = dataArrayPtr[0x20 + (indexTwo * 12) + i];
        dataArrayPtr[0x20 + (indexTwo * 12) + i] = tempByte;
    }

    int valOne;
    int valTwo;
    int tempInt;

    for (int i = 0; i < 4; i++)
    {
        if (substructOffsets[i] == indexOne * 12)
        {
            valOne = i;
        }
        if (substructOffsets[i] == indexTwo * 12)
        {
            valTwo = i;
        }
    }
    tempInt = substructOffsets[valOne];
    substructOffsets[valOne] = substructOffsets[valTwo];
    substructOffsets[valTwo] = tempInt;
}

byte Gen3Pokemon::getUnownLetter()
{
    if (getSpeciesIndexNumber() == 201)
    {

        u32 personalityValue = getPersonalityValue();
        return (
                   ((personalityValue & 0x03000000) >> 18) +
                   ((personalityValue & 0x00030000) >> 12) +
                   ((personalityValue & 0x00000300) >> 6) +
                   ((personalityValue & 0x00000003) >> 0)) %
               28;
    }
    else
    {
        return 255;
    }
};

Nature Gen3Pokemon::getNature()
{
    return (Nature)(getPersonalityValue() % 25);
};

Gender Gen3Pokemon::getGender()
{
    byte index = getSpeciesIndexNumber();
    int threshold = pokeTable->get_gender_threshold(index, true);

    switch (threshold)
    {
    case 0:
        return MALE;
    case 254:
        return FEMALE;
    case 255:
        return GENDERLESS;
    default:
        if ((getPersonalityValue() & 0xFF) >= threshold)
        {
            return MALE;
        }
        return FEMALE;
    }
}
bool Gen3Pokemon::getIsShiny()
{
    return (getTrainerID() ^
            getSecretID() ^
            (getPersonalityValue() >> 0 & 0xFFFF) ^
            (getPersonalityValue() >> 16 & 0xFFFF)) < 8;
}

bool Gen3Pokemon::setNicknameArray(byte nameArr[], int nameArrSize)
{
    for (int i = 0; i < nameArrSize; i++)
    {
        setNicknameLetter(i, nameArr[i]);
    }
    return true;
}

bool Gen3Pokemon::setOTArray(byte otArr[], int otArrSize)
{
    for (int i = 0; i < otArrSize; i++)
    {
        setOTLetter(i, otArr[i]);
    }
    return true;
}