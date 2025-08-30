#include "Gen3Pokemon.h"

Gen3Pokemon::Gen3Pokemon()
{
    dataArrayPtr = dataArray;
    dataArraySize = 80;
    isBigEndian = false;
};

bool Gen3Pokemon::convertToGen3(Gen3Pokemon *g3p)
{
    return false;
}

// This is used to easily print out a Pokemon, when using a standard C++ terminal
#if INCLUDE_IOSTREAM
void Gen3Pokemon::print(std::ostream &os)
{
    os
        << "Personality Value: " << std::hex << getPersonalityValue() << std::dec
        << "\n\tLetter: " << (int)getUnownLetter()
        << "\n\tNature: " << getNature()
        << "\n\tGender: " << getGender() << '\n'
        << "Trainer ID: " << getTrainerID() << "\n"
        << "Secret ID: " << getSecretID() << "\n"
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
        << "Fateful Encounter/Obedience: " << getFatefulEncounterObedience() << "\n";
};
#endif

// Get Nickname is different
// u32 getNickname() {};
// u32 getOriginalTrainerName() {}

u32 Gen3Pokemon::getNextRand_u32()
{
    currRand = (0x41C64E6D * currRand) + 0x6073;
    //std::cout << "Rolled: " << std::hex << currRand << "\n";
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

// This is used to load our data in from an array and decrypt it
void Gen3Pokemon::loadData(byte incomingArray[])
{
    for (int i = 0; i < dataArraySize; i++)
    {
        dataArrayPtr[i] = incomingArray[i];
    }
    decryptSubstructures();
}

// And then some general functions
void Gen3Pokemon::decryptSubstructures()
{
    u32 key = (getTrainerID() | getSecretID() << 16) ^ getPersonalityValue();
    for (int i = 0; i < 48; i++)
    {
        dataArrayPtr[0x20 + i] ^= ((key >> (8 * (i % 4))) & 0xFF);
    }
};

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
    PokemonTables data_tables;
    byte index = getSpeciesIndexNumber();
    int threshold = data_tables.get_gender_threshold(index, true);

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