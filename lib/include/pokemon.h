#ifndef GBPOKEMON_H
#define GBPOKEMON_H

#define INCLUDE_IOSTREAM true
#define USE_EXTERNAL_DATA false
#define USE_CPP_RAND true

#include "typeDefs.h"

#if INCLUDE_IOSTREAM
#include <iostream>
#include <sstream>
#endif

#if !USE_EXTERNALDATA
#include "pokemonData.h"
#endif

#if USE_CPP_RAND

#endif

// Avoid having to import math
u32 sizeToMask(int len)
{
    u32 out = 1;
    for (int i = 0; i < len; i++)
    {
        out *= 2;
    }
    return (out - 1);
}

// For our random functions
#define RAND_U16(v) (v >> 16)

#if USE_CPP_RAND
// #include <random>
#endif

inline u32 getNextRand(u32 *currRand)
{
    *currRand = (0x41C64E6D * *currRand) + 0x6073;
    return *currRand;
}

inline u32 getPrevRand(u32 *currRand)
{
    *currRand = (*currRand - 24691) * 4005161829;
    return *currRand;
}

inline u32 getPureRand() // Gets a random number from the device itself
{
#if USE_CPP_RAND
    srand(time(0));
    return rand() << 16 | rand();
#else

#endif
}

// FNV-1a 32-bit hash function for byte arrays
u32 fnv1a_hash(unsigned char *data, size_t length)
{
    const uint32_t fnv_prime = 0x01000193;
    const uint32_t fnv_offset_basis = 0x811C9DC5;
    uint32_t hash = fnv_offset_basis;

    for (size_t i = 0; i < length; ++i)
    {
        hash ^= data[i];
        hash *= fnv_prime;
    }
    return hash;
}

class Pokemon // The base Pokemon class
{

public:
    // This is used to load our data in from an array
    void loadData(byte incomingArray[])
    {
        for (int i = 0; i < dataArraySize; i++)
        {
            dataArrayPtr[i] = incomingArray[i];
        }
    }

#if INCLUDE_IOSTREAM
    virtual void print(std::ostream &os)
    {
        os << "This is a base Pokemon, it has no info!";
    };
    friend std::ostream &operator<<(std::ostream &os, Pokemon &p)
    {
        p.print(os);
        return os;
    }
#endif

protected:
    //  Will be set by child classes
    byte *dataArrayPtr;
    int dataArraySize;

    // These can probably move
    byte *nicknameArrayPtr;
    int nicknameArraySize;

    byte *OTArrayPtr;
    int OTArraySize;

    byte *externalIndexNumberPtr;

    bool isBigEndian;

    u32 getVar(DataVarInfo dataVar)
    {
        return getVar(dataVar, 0);
    }

    u32 getVar(DataVarInfo dataVar, int extraByteOffset)
    {
        u32 out = 0;
        if (dataVar.dataLength < 8)
        { // is less than a byte, do bitwise stuff on a single byte
            // ... but we can't assume that the data is within a single byte (thanks gen 3)
            if (dataVar.dataLength + dataVar.bitOffset > 8)
            {
                int numBytes = (dataVar.dataLength + dataVar.bitOffset) / 8 + 1;
                if (numBytes > 4)
                {
                    numBytes = 4; // This avoids importing math for rounding. Silly though.
                }
                int arrayIndex;
                for (int i = 0; i < numBytes; i++)
                {
                    arrayIndex = (isBigEndian ? i : numBytes - (i + 1));
                    out = (out << 8) | dataArrayPtr[dataVar.byteOffset + extraByteOffset + arrayIndex];
                }
                out = (out >> dataVar.bitOffset) & sizeToMask(dataVar.dataLength);
            }
            else
            {
                out = (dataArrayPtr[dataVar.byteOffset + extraByteOffset] >> dataVar.bitOffset) & sizeToMask(dataVar.dataLength);
            }
        }
        else
        { // is larger than a byte, will have to access multiple parts of the array
            int numBytes = dataVar.dataLength / 8;
            int arrayIndex;
            for (int i = 0; i < numBytes; i++)
            {
                arrayIndex = (isBigEndian ? i : numBytes - (i + 1));
                out = (out << 8) | dataArrayPtr[dataVar.byteOffset + extraByteOffset + arrayIndex];
            }
        }
        return out;
    }

    bool setVar(DataVarInfo dataVar, u32 newValue)
    {
        return setVar(dataVar, 0, newValue);
    }

    bool setVar(DataVarInfo dataVar, int extraByteOffset, u32 newValue)
    {
        if (dataVar.dataLength < 8)
        { // is less than a byte, do bitwise stuff on a single byte
          // ... but we can't assume that the data is within a single byte (thanks gen 3)
            if (dataVar.dataLength + dataVar.bitOffset > 8)
            {
                int numBytes = (dataVar.dataLength + dataVar.bitOffset) / 8 + 1;
                if (numBytes > 4)
                {
                    numBytes = 4; // This avoids importing math for rounding. Silly though.
                }
                int arrayIndex;
                for (int i = 0; i < numBytes; i++)
                {
                    arrayIndex = (isBigEndian ? i : numBytes - (i + 1));
                    dataArrayPtr[dataVar.byteOffset + arrayIndex + extraByteOffset] &= ~((sizeToMask(dataVar.dataLength) << dataVar.bitOffset) >> (arrayIndex * 8));
                    dataArrayPtr[dataVar.byteOffset + arrayIndex + extraByteOffset] |= ((newValue & sizeToMask(dataVar.dataLength)) << dataVar.bitOffset) >> (arrayIndex * 8);
                }
            }
            else
            {
                dataArrayPtr[dataVar.byteOffset + extraByteOffset] &= ~(sizeToMask(dataVar.dataLength) << dataVar.bitOffset);
                dataArrayPtr[dataVar.byteOffset + extraByteOffset] |= (newValue & sizeToMask(dataVar.dataLength)) << dataVar.bitOffset;
            }
        }
        else
        { // is larger than a byte, will have to access multiple parts of the array
            int numBytes = dataVar.dataLength / 8;
            int arrayIndex;
            for (int i = 0; i < numBytes; i++)
            {
                arrayIndex = (isBigEndian ? i : numBytes - (i + 1));
                dataArrayPtr[dataVar.byteOffset + arrayIndex + extraByteOffset] = (newValue >> (8 * ((numBytes - 1) - i)));
            }
        }
        return true;
    }
};

class Gen3Pokemon : public Pokemon // The class for gen 3 Pokemon

{
public:
    Gen3Pokemon()
    {
        dataArrayPtr = dataArray;
        dataArraySize = 80;
        isBigEndian = false;
    };

    bool convertToGen3(Gen3Pokemon *g3p)
    {
        return false;
    }

protected:
    // These store the data bytes
    byte dataArray[80] = {0};

    // These store the offsets of the various data substructures:
    int substructOffset_G = 24;
    int substructOffset_A = 36;
    int substructOffset_E = 12;
    int substructOffset_M = 0;

#pragma region
    // Since there is only the Pokemon parent class, we can directly define these directly
    const DataVarInfo
        // All of the data info variables
        personalityValue =
            {0x00, 32, 0},
        trainerID =
            {0x04, 16, 0},
        secretID =
            {0x06, 16, 0},
        nicknameLetterOne =
            {0x08, 8, 0}, // This is silly. Change this.
        nicknameLetterTwo =
            {0x09, 8, 0},
        nicknameLetterThree =
            {0x0A, 8, 0},
        nicknameLetterFour =
            {0x0B, 8, 0},
        nicknameLetterFive =
            {0x0C, 8, 0},
        nicknameLetterSix =
            {0x0D, 8, 0},
        nicknameLetterSeven =
            {0x0E, 8, 0},
        nicknameLetterEight =
            {0x0F, 8, 0},
        nicknameLetterNine =
            {0x10, 8, 0},
        nicknameLetterTen =
            {0x11, 8, 0},
        language =
            {0x12, 8, 0},
        isBadEgg =
            {0x13, 1, 0},
        hasSpecies =
            {0x13, 1, 1},
        useEggName =
            {0x13, 1, 2},
        blockBoxRS =
            {0x13, 1, 3},
        // unusedFlags =
        //     {0x13, 4, 4},
        originalTrainerNameLetterOne =
            {0x14, 8, 0}, // This is also silly. Change this.
        originalTrainerNameLetterTwo =
            {0x15, 8, 0},
        originalTrainerNameLetterThree =
            {0x16, 8, 0},
        originalTrainerNameLetterFour =
            {0x17, 8, 0},
        originalTrainerNameLetterFive =
            {0x18, 8, 0},
        originalTrainerNameLetterSix =
            {0x19, 8, 0},
        originalTrainerNameLetterSeven =
            {0x1A, 8, 0},
        markings =
            {0x1B, 4, 0},
        checksum =
            {0x1C, 16, 0};
    // unknown =
    //     {0x1E, 16, 0};

    const DataVarInfo
        *nickname[10] = {
            &nicknameLetterOne,
            &nicknameLetterTwo,
            &nicknameLetterThree,
            &nicknameLetterFour,
            &nicknameLetterFive,
            &nicknameLetterSix,
            &nicknameLetterSeven,
            &nicknameLetterEight,
            &nicknameLetterNine,
            &nicknameLetterTen,
        },
        *originalTrainerName[7] = {
            &originalTrainerNameLetterOne,
            &originalTrainerNameLetterTwo,
            &originalTrainerNameLetterThree,
            &originalTrainerNameLetterFour,
            &originalTrainerNameLetterFive,
            &originalTrainerNameLetterSix,
            &originalTrainerNameLetterSeven,
        };

    // data section G
    const DataVarInfo
        speciesIndexNumber =
            {0x20 + 0x00, 16, 0},
        heldItem =
            {0x20 + 0x02, 16, 0},
        expPoints =
            {0x20 + 0x04, 32, 0},
        ppUpNumMoveOne =
            {0x20 + 0x08, 2, 0},
        ppUpNumMoveTwo =
            {0x20 + 0x08, 2, 2},
        ppUpNumMoveThree =
            {0x20 + 0x08, 2, 4},
        ppUpNumMoveFour =
            {0x20 + 0x08, 2, 6},
        friendship =
            {0x20 + 0x09, 8, 0};
    // unused =
    //    {0x20 + 0x0A, 16, 0};
    const DataVarInfo
        *ppUpNums[4] = {
            &ppUpNumMoveOne,
            &ppUpNumMoveTwo,
            &ppUpNumMoveThree,
            &ppUpNumMoveFour,
        };

    // data section A
    const DataVarInfo
        moveOne =
            {0x20 + 0x00, 16, 0},
        moveTwo =
            {0x20 + 0x02, 16, 0},
        moveThree =
            {0x20 + 0x04, 16, 0},
        moveFour =
            {0x20 + 0x06, 16, 0},
        moveOnePP =
            {0x20 + 0x08, 8, 0},
        moveTwoPP =
            {0x20 + 0x09, 8, 0},
        moveThreePP =
            {0x20 + 0x0A, 8, 0},
        moveFourPP =
            {0x20 + 0x0B, 8, 0};
    const DataVarInfo
        *moves[4] = {
            &moveOne,
            &moveTwo,
            &moveThree,
            &moveFour,
        },
        *ppUpTotals[4] = {
            &moveOnePP,
            &moveTwoPP,
            &moveThreePP,
            &moveFourPP,
        };

    // data section E
    const DataVarInfo
        hpEVs =
            {0x20 + 0x00, 8, 0},
        attackEVs =
            {0x20 + 0x01, 8, 0},
        defenseEVs =
            {0x20 + 0x02, 8, 0},
        speedEVs =
            {0x20 + 0x03, 8, 0},
        specialAttackEVs =
            {0x20 + 0x04, 8, 0},
        specialDefenseEVs =
            {0x20 + 0x05, 8, 0},
        coolnessCondition =
            {0x20 + 0x06, 8, 0},
        beautyCondition =
            {0x20 + 0x07, 8, 0},
        cutenessCondition =
            {0x20 + 0x08, 8, 0},
        smartnessCondition =
            {0x20 + 0x09, 8, 0},
        toughnessCondition =
            {0x20 + 0x0A, 8, 0},
        sheen =
            {0x20 + 0x0B, 8, 0};

    const DataVarInfo
        *EVs[6] = {
            &hpEVs,
            &attackEVs,
            &defenseEVs,
            &speedEVs,
            &specialAttackEVs,
            &specialDefenseEVs,
        },
        *contestConditions[5] = {
            &coolnessCondition,
            &beautyCondition,
            &cutenessCondition,
            &smartnessCondition,
            &toughnessCondition,
        };

    const DataVarInfo

        // data section M
        pokerusStrain =
            {0x20 + 0x00, 4, 0},
        pokerusDaysRemaining =
            {0x20 + 0x00, 4, 4},
        metLocation =
            {0x20 + 0x01, 8, 0},
        levelMet =
            {0x20 + 0x02, 7, 0},
        gameOfOrigin =
            {0x20 + 0x02, 4, 7},
        pokeballCaughtIn =
            {0x20 + 0x02, 4, 11},
        originalTrainerGender =
            {0x20 + 0x02, 1, 15},
        hpIVs =
            {0x20 + 0x04, 5, 0},
        attackIVs =
            {0x20 + 0x04, 5, 5},
        defenseIVs =
            {0x20 + 0x04, 5, 10},
        speedIVs =
            {0x20 + 0x04, 5, 15},
        specialAttackIVs =
            {0x20 + 0x04, 5, 20},
        specialDefenseIVs =
            {0x20 + 0x04, 5, 25},
        isEgg =
            {0x20 + 0x04, 1, 30},
        ability =
            {0x20 + 0x04, 1, 31},
        coolNormalContestRibbon =
            {0x20 + 0x08, 1, 0}, // This is also very silly. Change it.
        coolSuperContestRibbon =
            {0x20 + 0x08, 1, 0},
        coolHyperContestRibbon =
            {0x20 + 0x08, 1, 1},
        coolMasterContestRibbon =
            {0x20 + 0x08, 1, 2},
        beautyNormalContestRibbon =
            {0x20 + 0x08, 1, 3},
        beautySuperContestRibbon =
            {0x20 + 0x08, 1, 4},
        beautyHyperContestRibbon =
            {0x20 + 0x08, 1, 5},
        beautyMasterContestRibbon =
            {0x20 + 0x08, 1, 6},
        cuteNormalContestRibbon =
            {0x20 + 0x08, 1, 7},
        cuteSuperContestRibbon =
            {0x20 + 0x08, 1, 8},
        cuteHyperContestRibbon =
            {0x20 + 0x08, 1, 9},
        cuteMasterContestRibbon =
            {0x20 + 0x08, 1, 10},
        smartNormalContestRibbon =
            {0x20 + 0x08, 1, 11},
        smartSuperContestRibbon =
            {0x20 + 0x08, 1, 12},
        smartHyperContestRibbon =
            {0x20 + 0x08, 1, 13},
        smartMasterContestRibbon =
            {0x20 + 0x08, 1, 14},
        toughNormalContestRibbon =
            {0x20 + 0x08, 1, 15},
        toughSuperContestRibbon =
            {0x20 + 0x08, 1, 16},
        toughHyperContestRibbon =
            {0x20 + 0x08, 1, 17},
        toughMasterContestRibbon =
            {0x20 + 0x08, 1, 18},
        championRibbon =
            {0x20 + 0x08, 1, 19},
        winningRibbon =
            {0x20 + 0x08, 1, 20},
        victoryRibbon =
            {0x20 + 0x08, 1, 21},
        artistRibbon =
            {0x20 + 0x08, 1, 22},
        effortRibbon =
            {0x20 + 0x08, 1, 23},
        battleChampionRibbon =
            {0x20 + 0x08, 1, 24},
        regionalChampionRibbon =
            {0x20 + 0x08, 1, 25},
        nationalChampionRibbon =
            {0x20 + 0x08, 1, 26},
        countryRibbon =
            {0x20 + 0x08, 1, 27},
        nationalRibbon =
            {0x20 + 0x08, 1, 28},
        earthRibbon =
            {0x20 + 0x08, 1, 29},
        unusedRibbons =
            {0x20 + 0x08, 1, 30},
        fatefulEncounterObedience =
            {0x20 + 0x08, 1, 31};

    const DataVarInfo
        *IVs[6] = {
            &hpIVs,
            &attackIVs,
            &defenseIVs,
            &speedIVs,
            &specialAttackIVs,
            &specialDefenseIVs,
        },
        *ribbons[31] = {
            &coolNormalContestRibbon,
            &coolSuperContestRibbon,
            &coolHyperContestRibbon,
            &coolMasterContestRibbon,
            &beautyNormalContestRibbon,
            &beautySuperContestRibbon,
            &beautyHyperContestRibbon,
            &beautyMasterContestRibbon,
            &cuteNormalContestRibbon,
            &cuteSuperContestRibbon,
            &cuteHyperContestRibbon,
            &cuteMasterContestRibbon,
            &smartNormalContestRibbon,
            &smartSuperContestRibbon,
            &smartHyperContestRibbon,
            &smartMasterContestRibbon,
            &toughNormalContestRibbon,
            &toughSuperContestRibbon,
            &toughHyperContestRibbon,
            &toughMasterContestRibbon,
            &championRibbon,
            &winningRibbon,
            &victoryRibbon,
            &artistRibbon,
            &effortRibbon,
            &battleChampionRibbon,
            &regionalChampionRibbon,
            &nationalChampionRibbon,
            &countryRibbon,
            &nationalRibbon,
            &earthRibbon,
        };
#pragma endregion

    // This is used to easily print out a Pokemon, when using a standard C++ terminal
#if INCLUDE_IOSTREAM
    void print(std::ostream &os)
    {
        os << "\n"
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
           << "\n\tSpeed: " << getEV(SPEED) << " EVs, " << getIV(SPEED) << " IVs"
           << "\n\tSpecial Attack: " << getEV(SPECIAL_ATTACK) << " EVs, " << getIV(SPECIAL_ATTACK) << " IVs"
           << "\n\tSpecial Defense: " << getEV(SPECIAL_DEFENSE) << " EVs, " << getIV(SPECIAL_DEFENSE) << " IVs" << "\n" // Special Attack and Special Defense are the same
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

public:
    // All of the simple getters and setters are defined here
    u32 getPersonalityValue() { return getVar(personalityValue); }
    u32 getTrainerID() { return getVar(trainerID); }
    u32 getSecretID() { return getVar(secretID); }
    u32 getLanguage() { return getVar(language); }
    u32 getIsBadEgg() { return getVar(isBadEgg); }
    u32 getHasSpecies() { return getVar(hasSpecies); }
    u32 getUseEggName() { return getVar(useEggName); }
    u32 getBlockBoxRS() { return getVar(blockBoxRS); }
    u32 getMarkings() { return getVar(markings); }
    u32 getChecksum() { return getVar(checksum); }
    u32 getSpeciesIndexNumber() { return getVar(speciesIndexNumber, substructOffset_G); }
    u32 getHeldItem() { return getVar(heldItem, substructOffset_G); }
    u32 getExpPoints() { return getVar(expPoints, substructOffset_G); }
    u32 getFriendship() { return getVar(friendship, substructOffset_G); }
    u32 getSheen() { return getVar(sheen, substructOffset_E); }
    u32 getPokerusStrain() { return getVar(pokerusStrain, substructOffset_M); }
    u32 getPokerusDaysRemaining() { return getVar(pokerusDaysRemaining, substructOffset_M); }
    u32 getMetLocation() { return getVar(metLocation, substructOffset_M); }
    u32 getLevelMet() { return getVar(levelMet, substructOffset_M); }
    u32 getGameOfOrigin() { return getVar(gameOfOrigin, substructOffset_M); }
    u32 getPokeballCaughtIn() { return getVar(pokeballCaughtIn, substructOffset_M); }
    u32 getOriginalTrainerGender() { return getVar(originalTrainerGender, substructOffset_M); }
    u32 getIsEgg() { return getVar(isEgg, substructOffset_M); }
    u32 getAbility() { return getVar(ability, substructOffset_M); }
    u32 getFatefulEncounterObedience() { return getVar(fatefulEncounterObedience, substructOffset_M); }

    bool setTrainerID(byte newVal) { return setVar(trainerID, newVal); }
    bool setSecretID(byte newVal) { return setVar(secretID, newVal); }
    bool setLanguage(byte newVal) { return setVar(language, newVal); }
    bool setIsBadEgg(byte newVal) { return setVar(isBadEgg, newVal); }
    bool setHasSpecies(byte newVal) { return setVar(hasSpecies, newVal); }
    bool setUseEggName(byte newVal) { return setVar(useEggName, newVal); }
    bool setBlockBoxRS(byte newVal) { return setVar(blockBoxRS, newVal); }
    bool setMarkings(byte newVal) { return setVar(markings, newVal); }
    bool setChecksum(byte newVal) { return setVar(checksum, newVal); }
    bool setSpeciesIndexNumber(byte newVal) { return setVar(speciesIndexNumber, substructOffset_G, newVal); }
    bool setHeldItem(byte newVal) { return setVar(heldItem, substructOffset_G, newVal); }
    bool setExpPoints(byte newVal) { return setVar(expPoints, substructOffset_G, newVal); }
    bool setFriendship(byte newVal) { return setVar(friendship, substructOffset_G, newVal); }
    bool setSheen(byte newVal) { return setVar(sheen, substructOffset_E, newVal); }
    bool setPokerusStrain(byte newVal) { return setVar(pokerusStrain, substructOffset_M, newVal); }
    bool setPokerusDaysRemaining(byte newVal) { return setVar(pokerusDaysRemaining, substructOffset_M, newVal); }
    bool setMetLocation(byte newVal) { return setVar(metLocation, substructOffset_M, newVal); }
    bool setLevelMet(byte newVal) { return setVar(levelMet, substructOffset_M, newVal); }
    bool setGameOfOrigin(byte newVal) { return setVar(gameOfOrigin, substructOffset_M, newVal); }
    bool setPokeballCaughtIn(byte newVal) { return setVar(pokeballCaughtIn, substructOffset_M, newVal); }
    bool setOriginalTrainerGender(byte newVal) { return setVar(originalTrainerGender, substructOffset_M, newVal); }
    bool setIsEgg(byte newVal) { return setVar(isEgg, substructOffset_M, newVal); }
    bool setAbility(byte newVal) { return setVar(ability, substructOffset_M, newVal); }
    bool setFatefulEncounterObedience(byte newVal) { return setVar(fatefulEncounterObedience, substructOffset_M, newVal); }

    // The ones that access arrays are defined here:
    u32 getPPUpNum(int moveIndex) { return getVar(*ppUpNums[moveIndex], substructOffset_G); }
    u32 getMove(int moveIndex) { return getVar(*moves[moveIndex], substructOffset_A); }
    u32 getPPTotal(int moveIndex) { return getVar(*ppUpTotals[moveIndex], substructOffset_A); }
    u32 getEV(Stat currStat) { return getVar(*EVs[currStat], substructOffset_E); }
    u32 getContestCondition(Condition currCondition) { return getVar(*contestConditions[currCondition], substructOffset_E); }
    u32 getIV(Stat currStat) { return getVar(*IVs[currStat], substructOffset_M); }
    u32 getRibbons(Ribbon currRibbon) { return getVar(*ribbons[currRibbon], substructOffset_M); }

    bool setPPUpNum(int moveIndex, byte newVal) { return setVar(*ppUpNums[moveIndex], substructOffset_G, newVal); }
    bool setMove(int moveIndex, byte newVal) { return setVar(*moves[moveIndex], substructOffset_A, newVal); }
    bool setPPTotal(int moveIndex, byte newVal) { return setVar(*ppUpTotals[moveIndex], substructOffset_A, newVal); }
    bool setEV(Stat currStat, byte newVal) { return setVar(*EVs[currStat], substructOffset_E, newVal); }
    bool setContestCondition(Condition currCondition, byte newVal) { return setVar(*contestConditions[currCondition], substructOffset_E, newVal); }
    bool setIV(Stat currStat, byte newVal) { return setVar(*IVs[currStat], substructOffset_M, newVal); }
    bool setRibbons(Ribbon currRibbon, byte newVal) { return setVar(*ribbons[currRibbon], substructOffset_M, newVal); }

    // Get Nickname is different
    // u32 getNickname() {};
    // u32 getOriginalTrainerName() {}

    bool setPersonalityValue(u32 newVal) // Setting the Personality Value should modify some other values as well
    {
        bool successful = setVar(personalityValue, newVal);
        if (successful)
        {
            successful &= setAbility(getPersonalityValue() & 0b1);
        }
        return successful;
    }

    // This is used to load our data in from an array and decrypt it
    void loadData(byte incomingArray[])
    {
        for (int i = 0; i < dataArraySize; i++)
        {
            dataArrayPtr[i] = incomingArray[i];
        }
        decryptSubstructures();
    }

    // And then some general functions
    void decryptSubstructures()
    {
        u32 key = (getTrainerID() | getSecretID() << 16) ^ getPersonalityValue();
        for (int i = 0; i < 48; i++)
        {
            dataArrayPtr[0x20 + i] ^= ((key >> (8 * (i % 4))) & 0xFF);
        }
    };

    byte getUnownLetter()
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

    Nature getNature()
    {
        return (Nature)(getPersonalityValue() % 25);
    };

    Gender getGender()
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
};

class GBPokemon : public Pokemon // The class for gen 1 and 2 Pokemon, since they share some conversion functions
{
protected:
    // This constructor fills all our convenience arrays
    GBPokemon()
    {
        moves[0] = &moveOne, moves[1] = &moveTwo, moves[2] = &moveThree, moves[3] = &moveFour;
        statExps[0] = &hpStatExp, statExps[1] = &atkStatExp, statExps[2] = &defStatExp, statExps[3] = &speStatExp, statExps[4] = &spcStatExp;
        DVs[1] = &atkDV, DVs[2] = &defDV, DVs[3] = &speDV, DVs[4] = &spcDV;
        PPUpNums[0] = &ppUpNumMoveOne, PPUpNums[1] = &ppUpNumMoveTwo, PPUpNums[2] = &ppUpNumMoveThree, PPUpNums[3] = &ppUpNumMoveFour;
        PPUpTotals[0] = &ppNumTotalMoveOne, PPUpTotals[1] = &ppNumTotalMoveTwo, PPUpTotals[2] = &ppNumTotalMoveThree, PPUpTotals[3] = &ppNumTotalMoveFour;

        g1_types[0] = &g1_typeOne, g1_types[1] = &g1_typeTwo;

        nicknameArrayPtr = nicknameArray;
        OTArrayPtr = OTArray;
        externalIndexNumberPtr = &externalIndexNumber;
        isBigEndian = true;
    }

    // These store the data bytes
    byte nicknameArray[11];
    byte OTArray[11];
    byte externalIndexNumber;

    // These will be filled by each gen child class with the correct data location
    DataVarInfo
        // All of the data info variables
        speciesIndexNumber,
        level,
        moveOne, moveTwo, moveThree, moveFour,
        trainerID,
        expPoints,
        hpStatExp, atkStatExp, defStatExp, speStatExp, spcStatExp,
        atkDV, defDV, speDV, spcDV,
        ppUpNumMoveOne, ppUpNumMoveTwo, ppUpNumMoveThree, ppUpNumMoveFour,
        ppNumTotalMoveOne, ppNumTotalMoveTwo, ppNumTotalMoveThree, ppNumTotalMoveFour,

        // Gen 1 specific data
        g1_currentHP,
        g1_statusCondition,
        g1_typeOne, g1_typeTwo,
        g1_catchRate,

        // Gen 2 specific data
        g2_heldItem,
        g2_friendship,
        g2_pokerusStrain, g2_pokerusDaysRemaining,
        g2_caughtDataTime,
        g2_caughtDataLevel,
        g2_caughtDataGender,
        g2_caughtDataLocation;

    // and the convenient arrays of some of the variable groups:
    DataVarInfo
        *g1_types[2],
        *moves[4],
        *statExps[5],
        *DVs[5],
        *PPUpNums[4],
        *PPUpTotals[4];

    // This is used to easily print out a Pokemon, when using a standard C++ terminal
#if INCLUDE_IOSTREAM
    std::string parentPrint()
    {
        std::stringstream ss;
        ss << "Species Index Number: " << getSpeciesIndexNumber() << "\n"
           << "Trainer ID: " << getTrainerID() << "\n"
           << "Level: " << getLevel() << "\n"
           << "Exp: " << getExpPoints() << "\n"
           << "Moves: "
           << "\n\t" << getMove(0) << " (" << getPPTotal(0) << " PP, " << getPPUpNum(0) << " PP Ups" << ")"
           << "\n\t" << getMove(1) << " (" << getPPTotal(1) << " PP, " << getPPUpNum(1) << " PP Ups" << ")"
           << "\n\t" << getMove(2) << " (" << getPPTotal(2) << " PP, " << getPPUpNum(2) << " PP Ups" << ")"
           << "\n\t" << getMove(3) << " (" << getPPTotal(3) << " PP, " << getPPUpNum(3) << " PP Ups" << ")" << "\n";
        return ss.str();
    }
#endif

public:
    // All of the simple getters and setters are defined here
    u32 getLevel() { return getVar(level); }
    u32 getSpeciesIndexNumber() { return getVar(speciesIndexNumber); }
    u32 getTrainerID() { return getVar(trainerID); }
    u32 getExpPoints() { return getVar(expPoints); }
    virtual u32 getHeldItem() = 0;

    bool setLevel(byte newVal) { return setVar(level, newVal); }
    bool setSpeciesIndexNumber(byte newVal) { return setVar(speciesIndexNumber, newVal); }
    bool setOriginalTrainerID(byte newVal) { return setVar(trainerID, newVal); }
    bool setExpPoints(byte newVal) { return setVar(expPoints, newVal); }

    // The ones that access arrays are defined here:
    u32 getMove(int moveIndex) { return getVar(*moves[moveIndex]); }
    u32 getStatExp(Stat currStat) { return getVar(*statExps[currStat]); }
    u32 getPPUpNum(int moveIndex) { return getVar(*PPUpNums[moveIndex]); }
    u32 getPPTotal(int moveIndex) { return getVar(*PPUpTotals[moveIndex]); }

    bool setMove(int moveIndex, byte newVal) { return setVar(*moves[moveIndex], newVal); }
    bool setStatExp(Stat currStat, byte newVal) { return setVar(*statExps[currStat], newVal); }
    bool setPPUpNum(int moveIndex, byte newVal) { return setVar(*PPUpNums[moveIndex], newVal); }
    bool setPPTotal(int moveIndex, byte newVal) { return setVar(*PPUpTotals[moveIndex], newVal); }

    // Get DV is special due to the implementation of HP DV
    u32 getDV(Stat currStat)
    {
        if (currStat == HP)
        {

            return ((getVar(*DVs[ATTACK]) & 0x1) << 3) |
                   ((getVar(*DVs[DEFENSE]) & 0x1) << 2) |
                   ((getVar(*DVs[SPEED]) & 0x1) << 1) |
                   ((getVar(*DVs[SPECIAL]) & 0x1) << 0);
        }
        else
        {
            return getVar(*DVs[currStat]);
        }
    }

    bool setDV(Stat currStat, byte newVal)
    {
        if (currStat == HP)
        {
            return setVar(*DVs[ATTACK], (getVar(*DVs[ATTACK]) & 0b1110) | ((newVal >> 3) & 0x1)) &&
                   setVar(*DVs[DEFENSE], (getVar(*DVs[DEFENSE]) & 0b1110) | ((newVal >> 2) & 0x1)) &&
                   setVar(*DVs[SPEED], (getVar(*DVs[SPEED]) & 0b1110) | ((newVal >> 1) & 0x1)) &&
                   setVar(*DVs[SPECIAL], (getVar(*DVs[SPECIAL]) & 0b1110) | ((newVal >> 0) & 0x1));
        }
        else
        {
            return setVar(*DVs[currStat], newVal);
        }
    }

    byte getUnownLetter()
    {
        if (getSpeciesIndexNumber() == 201)
        {
            byte letter = 0;
            letter |= ((getDV(ATTACK) & 0b0110) >> 1) << 6;
            letter |= ((getDV(DEFENSE) & 0b0110) >> 1) << 4;
            letter |= ((getDV(SPEED) & 0b0110) >> 1) << 2;
            letter |= ((getDV(SPECIAL) & 0b0110) >> 1) << 0;
            letter = letter / 10;
            return letter;
        }
        return 255;
    }

    Gender getGender()
    {
        PokemonTables data_tables;
        byte index = getSpeciesIndexNumber();
        int threshold = data_tables.get_gender_threshold(index, false);

        if (threshold == 255)
        {
            return GENDERLESS;
        }
        else
        {
            if (getDV(ATTACK) <= threshold)
            {
                return FEMALE;
            }
            return MALE;
        }
    }

    Nature getVirtualConsoleNature()
    {
        return (Nature)(getExpPoints() % 25);
    }

    bool convertToGen3(Gen3Pokemon *newPkmn)
    {
        return checkIfIsInvalid(newPkmn) &&
               setNewSpeciesIndex(newPkmn) &&
               generatePersonalityValue(newPkmn) &&
               // convertNickname(newPkmn);
               // convertTrainerName(newPkmn);
               checkEXP(newPkmn) &&
               checkIsShiny(newPkmn);
        // updateMoves(newPkmn);
        // setOriginInfo(newPkmn);
        // setRibbonsAndObedience(newPkmn);
        // setEVsAndIVs(newPkmn);
    };

    bool checkIfIsInvalid(Gen3Pokemon *newPkmn)
    {
        byte currSpeciesIndexNumber = getSpeciesIndexNumber();
        return currSpeciesIndexNumber <= 253 && // Checks if the Pokemon is beyond the spported Pokemon, Treecko included
               currSpeciesIndexNumber != 0 &&   // Makes sure the Pokemon isn't a blank party space
               // currSpeciesIndexNumber == externalIndexNumber && // Checks that the Pokemon isn't a hybrid or an egg
               getHeldItem() == 0; // Makes sure the current Pokemon doesn't have a held item
    };

    virtual bool setNewSpeciesIndex(Gen3Pokemon *newPkmn) = 0;

    bool generatePersonalityValue(Gen3Pokemon *newPkmn)
    {
        // Store these so we don't have to check every time.
        byte unownLetter = getUnownLetter();
        Nature virtualConsoleNature = getVirtualConsoleNature();
        Gender gender = getGender();
        u32 rand = getPureRand();
        do
        {
            rand = RAND_U16(rand) | RAND_U16(getNextRand(&rand)) << 16;
            newPkmn->setPersonalityValue(rand);
            /*
            std::cout << "PV: " << newPkmn->getPersonalityValue() << "\n"
                      << "Letter: " << newPkmn->getUnownLetter() << " | " << getUnownLetter() << "\n"
                      << "Nature: " << newPkmn->getNature() << " | " << getVirtualConsoleNature() << "\n"
                      << "Gender: " << newPkmn->getGender() << " | " << getGender() << "\n";
                      */
        } while (!(
            (newPkmn->getUnownLetter() == unownLetter) &&
            newPkmn->getNature() == virtualConsoleNature &&
            newPkmn->getGender() == gender));
        return true;
    };

    bool convertNickname(Gen3Pokemon *newPkmn)
    {
        return false;
    };
    bool convertTrainerName(Gen3Pokemon *newPkmn)
    {
        return false;
    };
    bool checkEXP(Gen3Pokemon *newPkmn)
    {
        PokemonTables data_tables;

        // Make sure Level is not over 100 based on EXP
        u32 maxExp = data_tables.get_max_exp(getSpeciesIndexNumber());
        if (getExpPoints() > maxExp)
        {
            setExpPoints(maxExp);
        }

        // Truncate the EXP down to the current level
        data_tables.load_exp_groups();
        int metLevel = getLevel();
        switch (data_tables.EXP_GROUPS[getSpeciesIndexNumber()])
        {
        case EXP_FAST:
            setExpPoints((4 * (metLevel * metLevel * metLevel)) / 5);
            break;

        case EXP_MED_FAST:
            setExpPoints(metLevel * metLevel * metLevel);
            break;

        case EXP_MED_SLOW:
            setExpPoints(((6 * metLevel * metLevel * metLevel) / 5) - (15 * metLevel * metLevel) + (100 * metLevel) - 140);
            break;

        case EXP_SLOW:
            setExpPoints((5 * (metLevel * metLevel * metLevel)) / 4);
            break;
        }
        return true;
    };
    bool checkIsShiny(Gen3Pokemon *newPkmn)
    {
        bool isShiny =
            (getDV(ATTACK) & 0b0010 == 0b0010) &&
            getDV(DEFENSE) == 10 &&
            getDV(SPEED) == 10 &&
            getDV(SPECIAL) == 10;

        if (getSpeciesIndexNumber() == 52 )//&&
            //fnv1a_hash(nickname, 7) == 1515822901 &&
            //(fnv1a_hash(trainer_name, 7) == 2671449886 || fnv1a_hash(trainer_name, 7) == 1342961308))
        {
            isShiny = true;
            for (int i = 1; i <= 4; i++)
            {
                setDV((Stat)i, 15);
            }
        }
        return false;
    };
    bool updateMoves(Gen3Pokemon *newPkmn)
    {
        return false;
    };
    bool setOriginInfo(Gen3Pokemon *newPkmn)
    {
        return false;
    };
    bool setRibbonsAndObedience(Gen3Pokemon *newPkmn)
    {
        return false;
    };
    bool setEVsAndIVs(Gen3Pokemon *newPkmn)
    {
        return false;
    };
};

class Gen1Pokemon : public GBPokemon // The class for gen 1 Pokemon
{
public:
    // All of this info is set in the constructor, so it is defined here instead of in the cpp file
    Gen1Pokemon()
    {
        dataArrayPtr = dataArray;
        dataArraySize = 33;

        speciesIndexNumber =
            {0x00, 8, 0};
        g1_currentHP =
            {0x01, 16, 0};
        level =
            {0x03, 8, 0};
        g1_statusCondition =
            {0x04, 8, 0};
        g1_typeOne =
            {0x05, 8, 0};
        g1_typeTwo =
            {0x06, 8, 0};
        g1_catchRate =
            {0x07, 8, 0};
        moveOne =
            {0x08, 8, 0};
        moveTwo =
            {0x09, 8, 0};
        moveThree =
            {0x0A, 8, 0};
        moveFour =
            {0x0B, 8, 0};
        trainerID =
            {0x0C, 16, 0};
        expPoints =
            {0x0E, 24, 0};
        hpStatExp =
            {0x11, 16, 0};
        atkStatExp =
            {0x13, 16, 0};
        defStatExp =
            {0x15, 16, 0};
        speStatExp =
            {0x17, 16, 0};
        spcStatExp =
            {0x19, 16, 0};
        atkDV =
            {0x1B, 4, 4};
        defDV =
            {0x1B, 4, 0};
        speDV =
            {0x1C, 4, 4};
        spcDV =
            {0x1C, 4, 0};
        ppUpNumMoveOne =
            {0x1D, 2, 6};
        ppNumTotalMoveOne =
            {0x1D, 6, 0};
        ppUpNumMoveTwo =
            {0x1E, 2, 6};
        ppNumTotalMoveTwo =
            {0x1E, 6, 0};
        ppUpNumMoveThree =
            {0x1F, 2, 6};
        ppNumTotalMoveThree =
            {0x1F, 6, 0};
        ppUpNumMoveFour =
            {0x20, 2, 6};
        ppNumTotalMoveFour =
            {0x20, 6, 0};
    }

    byte dataArray[33];

    u32 getCurrentHP() { return getVar(g1_currentHP); }
    u32 getStatusCondition() { return getVar(g1_statusCondition); }
    u32 getCatchRate() { return getVar(g1_catchRate); }
    u32 getHeldItem() { return 0; } // Gen 1 Pokemon will never have a held item. Silly, but this works nicely

    bool setCurrentHP(byte newVal) { return setVar(g1_currentHP, newVal); }
    bool setStatusCondition(byte newVal) { return setVar(g1_statusCondition, newVal); }
    bool setCatchRate(byte newVal) { return setVar(g1_catchRate, newVal); }

    u32 getType(int typeIndex) { return getVar(*g1_types[typeIndex]); }
    bool setType(int typeIndex, Gen1Types newVal) { return setVar(*g1_types[typeIndex], newVal); }

#if INCLUDE_IOSTREAM
    void print(std::ostream &os)
    {
        os << parentPrint()
           << "Stats: "
           << "\n\tHP: " << getStatExp(HP) << " Stat EXP, " << getDV(HP) << " DVs"
           << "\n\tAttack: " << getStatExp(ATTACK) << " Stat EXP, " << getDV(ATTACK) << " DVs"
           << "\n\tDefense: " << getStatExp(DEFENSE) << " Stat EXP, " << getDV(DEFENSE) << " DVs"
           << "\n\tSpeed: " << getStatExp(SPEED) << " Stat EXP, " << getDV(SPEED) << " DVs"
           << "\n\tSpecial: " << getStatExp(SPECIAL) << " Stat EXP, " << getDV(SPECIAL) << " DVs" << "\n"
           << "Types: "
           << "\n\t" << getType(0)
           << "\n\t" << getType(1) << "\n"
           << "Current HP: " << getCurrentHP() << "\n"
           << "Status Condition: " << getStatusCondition() << "\n"
           << "Catch Rate: " << getCatchRate() << "\n";
    }
#endif

    bool setNewSpeciesIndex(Gen3Pokemon *newPkmn)
    {
        newPkmn->setSpeciesIndexNumber(gen_1_index_array[getSpeciesIndexNumber()]);
        return true;
    }
};

class Gen2Pokemon : public GBPokemon // The class for gen 2 Pokemon
{
public:
    Gen2Pokemon()
    {
        dataArrayPtr = dataArray;
        dataArraySize = 32;

        speciesIndexNumber =
            {0x00, 8, 0};
        g2_heldItem =
            {0x01, 8, 0};
        moveOne =
            {0x02, 8, 0};
        moveTwo =
            {0x03, 8, 0};
        moveThree =
            {0x04, 8, 0};
        moveFour =
            {0x05, 8, 0};
        trainerID =
            {0x06, 16, 0};
        expPoints =
            {0x08, 24, 0};
        hpStatExp =
            {0x0B, 16, 0};
        atkStatExp =
            {0x0D, 16, 0};
        defStatExp =
            {0x0F, 16, 0};
        speStatExp =
            {0x11, 16, 0};
        spcStatExp =
            {0x13, 16, 0};
        atkDV =
            {0x15, 4, 4};
        defDV =
            {0x15, 4, 0};
        speDV =
            {0x16, 4, 4};
        spcDV =
            {0x16, 4, 0};
        ppUpNumMoveOne =
            {0x17, 2, 6};
        ppNumTotalMoveOne =
            {0x17, 6, 0};
        ppUpNumMoveTwo =
            {0x18, 2, 6};
        ppNumTotalMoveTwo =
            {0x18, 6, 0};
        ppUpNumMoveThree =
            {0x19, 2, 6};
        ppNumTotalMoveThree =
            {0x19, 6, 0};
        ppUpNumMoveFour =
            {0x1A, 2, 6};
        ppNumTotalMoveFour =
            {0x1A, 6, 0};
        g2_friendship =
            {0x1B, 8, 0};
        g2_pokerusStrain =
            {0x1C, 4, 4};
        g2_pokerusDaysRemaining =
            {0x1C, 4, 0};
        g2_caughtDataTime =
            {0x1D, 2, 6};
        g2_caughtDataLevel =
            {0x1D, 6, 0};
        g2_caughtDataGender =
            {0x1E, 1, 7};
        g2_caughtDataLocation =
            {0x1E, 7, 0};
        level =
            {0x1F, 8, 0};
    }

    byte dataArray[32];

    u32 getHeldItem() { return getVar(g2_heldItem); }
    u32 getFriendship() { return getVar(g2_friendship); }
    u32 getPokerusStrain() { return getVar(g2_pokerusStrain); }
    u32 getPokerusDaysRemaining() { return getVar(g2_pokerusDaysRemaining); }
    u32 getCaughtDataTime() { return getVar(g2_caughtDataTime); }
    u32 getCaughtDataLevel() { return getVar(g2_caughtDataLevel); }
    u32 getCaughtDataGender() { return getVar(g2_caughtDataGender); }
    u32 getCaughtDataLocation() { return getVar(g2_caughtDataLocation); }

    bool setHeldItem(byte newVal) { return setVar(g2_heldItem, newVal); }
    bool setFriendship(byte newVal) { return setVar(g2_friendship, newVal); }
    bool setPokerusStrain(byte newVal) { return setVar(g2_pokerusStrain, newVal); }
    bool setPokerusDaysRemaining(byte newVal) { return setVar(g2_pokerusDaysRemaining, newVal); }
    bool setCaughtDataTime(byte newVal) { return setVar(g2_caughtDataTime, newVal); }
    bool setCaughtDataLevel(byte newVal) { return setVar(g2_caughtDataLevel, newVal); }
    bool setCaughtDataGender(byte newVal) { return setVar(g2_caughtDataGender, newVal); }
    bool setCaughtDataLocation(byte newVal) { return setVar(g2_caughtDataLocation, newVal); }

#if INCLUDE_IOSTREAM
    void print(std::ostream &os)
    {
        os << parentPrint()
           << "Stats: "
           << "\n\tHP: " << getStatExp(HP) << " Stat EXP, " << getDV(HP) << " DVs"
           << "\n\tAttack: " << getStatExp(ATTACK) << " Stat EXP, " << getDV(ATTACK) << " DVs"
           << "\n\tDefense: " << getStatExp(DEFENSE) << " Stat EXP, " << getDV(DEFENSE) << " DVs"
           << "\n\tSpeed: " << getStatExp(SPEED) << " Stat EXP, " << getDV(SPEED) << " DVs"
           << "\n\tSpecial Attack: " << getStatExp(SPECIAL) << " Stat EXP, " << getDV(SPECIAL) << " DVs"
           << "\n\tSpecial Defense: " << getStatExp(SPECIAL) << " Stat EXP, " << getDV(SPECIAL) << " DVs" << "\n" // Special Attack and Special Defense are the same
           << "Held Item: " << getHeldItem() << "\n"
           << "Friendship: " << getFriendship() << "\n"
           << "Pokerus: "
           << "\n\tStrain: " << getPokerusStrain()
           << "\n\tDays Remaining: " << getPokerusDaysRemaining() << "\n"
           << "Caught Data: "
           << "\n\tTime: " << getCaughtDataTime()
           << "\n\tLevel: " << getCaughtDataLevel()
           << "\n\tTrainer Gender: " << getCaughtDataGender()
           << "\n\tLocation: " << getCaughtDataLocation();
    }
#endif

    bool setNewSpeciesIndex(Gen3Pokemon *newPkmn)
    {
        newPkmn->setSpeciesIndexNumber(getSpeciesIndexNumber());
        return true;
    }
};

class PokeBox // Stores up to 30 Pokemon in a box
{
private:
    Pokemon *boxStorage[30];
    int currIndex = 0;

public:
    bool addPokemon(Pokemon *currPkmn)
    {
        if (currIndex < 30)
        {
            boxStorage[currIndex] = currPkmn;
            currIndex++;
            return true;
        }
        return false;
    }

    Pokemon *getPokemon(int index)
    {
        if (index < currIndex)
        {
            return boxStorage[currIndex];
        }
        return nullptr;
    }

    bool removePokemon(int index)
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

#if INCLUDE_IOSTREAM
    friend std::ostream &operator<<(std::ostream &os, PokeBox &pc)
    {
        for (int i = 0; i < pc.currIndex; i++)
        {
            os << *pc.boxStorage[i] << "\n";
        }

        return os;
    }
#endif
};

#endif
