#ifndef GBPOKEMON_H
#define GBPOKEMON_H

#include "typeDefs.h"

#define INCLUDE_IOSTREAM true

#if INCLUDE_IOSTREAM
#include <iostream>
#include <sstream>
#endif

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

class Pokemon // The base Pokemon class
{
protected:
    //  Will be set by child classes
    byte *dataArrayPtr;
    int dataArraySize;

    byte *nicknameArrayPtr;
    int nicknameArraySize;

    byte *OTArrayPtr;
    int OTArraySize;

    byte *externalIndexNumberPtr;

    u32 getVar(DataVarInfo dataVar)
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
    bool setVar(DataVarInfo dataVar, int newValue)
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
    }

    // These store the data bytes
    byte nicknameArray[11];
    byte OTArray[11];
    byte externalIndexNumber;

    // These will be filled by each gen child class with the correct data location
    DataVarInfo
        // All of the data info variables
        speciesIndexNumber,
        moveOne, moveTwo, moveThree, moveFour,
        originalTrainerID,
        expPoints,
        hpStatExp, atkStatExp, defStatExp, speStatExp, spcStatExp,
        atkDV, defDV, speDV, spcDV,
        ppUpNumMoveOne, ppUpNumMoveTwo, ppUpNumMoveThree, ppUpNumMoveFour,
        ppNumTotalMoveOne, ppNumTotalMoveTwo, ppNumTotalMoveThree, ppNumTotalMoveFour,

        // Gen 1 specific data
        g1_currentHP,
        g1_pcLevel,
        g1_statusCondition,
        g1_typeOne, g1_typeTwo,
        g1_catchRate,

        // Gen 2 specific data
        g2_trueLevel,
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
    std::string parentCout()
    {
        std::stringstream ss;
        ss << "Species Index Number: " << getSpeciesIndexNumber() << "\n"
           << "Trainer ID: " << getOriginalTrainerID() << "\n"
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
    u32 getSpeciesIndexNumber() { return getVar(speciesIndexNumber); }
    u32 getOriginalTrainerID() { return getVar(originalTrainerID); }
    u32 getExpPoints() { return getVar(expPoints); }

    bool setSpeciesIndexNumber(byte newVal) { return setVar(speciesIndexNumber, newVal); }
    bool setOriginalTrainerID(byte newVal) { return setVar(originalTrainerID, newVal); }
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

    // This is used to load our data in from an array
    void loadData(byte incomingArray[])
    {
        for (int i = 0; i < dataArraySize; i++)
        {
            dataArrayPtr[i] = incomingArray[i];
        }
    }
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
        g1_pcLevel =
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
        originalTrainerID =
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
    u32 getPCLevel() { return getVar(g1_pcLevel); }
    u32 getStatusCondition() { return getVar(g1_statusCondition); }
    u32 getCatchRate() { return getVar(g1_catchRate); }

    bool setCurrentHP(byte newVal) { return setVar(g1_currentHP, newVal); }
    bool setPCLevel(byte newVal) { return setVar(g1_pcLevel, newVal); }
    bool setStatusCondition(byte newVal) { return setVar(g1_statusCondition, newVal); }
    bool setCatchRate(byte newVal) { return setVar(g1_catchRate, newVal); }

    u32 getType(int typeIndex) { return getVar(*g1_types[typeIndex]); }
    bool setType(int typeIndex, Gen1Types newVal) { return setVar(*g1_types[typeIndex], newVal); }

#if INCLUDE_IOSTREAM
    friend std::ostream &operator<<(std::ostream &os, Gen1Pokemon &m)
    {
        os << m.parentCout()
           << "Stats: "
           << "\n\tHP: " << m.getStatExp(HP) << " Stat EXP, " << m.getDV(HP) << " DVs"
           << "\n\tAttack: " << m.getStatExp(ATTACK) << " Stat EXP, " << m.getDV(ATTACK) << " DVs"
           << "\n\tDefense: " << m.getStatExp(DEFENSE) << " Stat EXP, " << m.getDV(DEFENSE) << " DVs"
           << "\n\tSpeed: " << m.getStatExp(SPEED) << " Stat EXP, " << m.getDV(SPEED) << " DVs"
           << "\n\tSpecial: " << m.getStatExp(SPECIAL) << " Stat EXP, " << m.getDV(SPECIAL) << " DVs" << "\n"
           << "Types: "
           << "\n\t" << m.getType(0)
           << "\n\t" << m.getType(1) << "\n"
           << "Current HP: " << m.getCurrentHP() << "\n"
           << "PC Level: " << m.getPCLevel() << "\n"
           << "Status Condition: " << m.getStatusCondition() << "\n"
           << "Catch Rate: " << m.getCatchRate() << "\n";
        return os;
    }
#endif
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
        originalTrainerID =
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
        g2_trueLevel =
            {0x1F, 8, 0};
    }

    byte dataArray[32];

    u32 getTrueLevel() { return getVar(g2_trueLevel); }
    u32 getHeldItem() { return getVar(g2_heldItem); }
    u32 getFriendship() { return getVar(g2_friendship); }
    u32 getPokerusStrain() { return getVar(g2_pokerusStrain); }
    u32 getPokerusDaysRemaining() { return getVar(g2_pokerusDaysRemaining); }
    u32 getCaughtDataTime() { return getVar(g2_caughtDataTime); }
    u32 getCaughtDataLevel() { return getVar(g2_caughtDataLevel); }
    u32 getCaughtDataGender() { return getVar(g2_caughtDataGender); }
    u32 getCaughtDataLocation() { return getVar(g2_caughtDataLocation); }

    bool setTrueLevel(byte newVal) { return setVar(g2_trueLevel, newVal); }
    bool setHeldItem(byte newVal) { return setVar(g2_heldItem, newVal); }
    bool setFriendship(byte newVal) { return setVar(g2_friendship, newVal); }
    bool setPokerusStrain(byte newVal) { return setVar(g2_pokerusStrain, newVal); }
    bool setPokerusDaysRemaining(byte newVal) { return setVar(g2_pokerusDaysRemaining, newVal); }
    bool setCaughtDataTime(byte newVal) { return setVar(g2_caughtDataTime, newVal); }
    bool setCaughtDataLevel(byte newVal) { return setVar(g2_caughtDataLevel, newVal); }
    bool setCaughtDataGender(byte newVal) { return setVar(g2_caughtDataGender, newVal); }
    bool setCaughtDataLocation(byte newVal) { return setVar(g2_caughtDataLocation, newVal); }

#if INCLUDE_IOSTREAM
    friend std::ostream &operator<<(std::ostream &os, Gen2Pokemon &m)
    {
        os << m.parentCout()
           << "Stats: "
           << "\n\tHP: " << m.getStatExp(HP) << " Stat EXP, " << m.getDV(HP) << " DVs"
           << "\n\tAttack: " << m.getStatExp(ATTACK) << " Stat EXP, " << m.getDV(ATTACK) << " DVs"
           << "\n\tDefense: " << m.getStatExp(DEFENSE) << " Stat EXP, " << m.getDV(DEFENSE) << " DVs"
           << "\n\tSpeed: " << m.getStatExp(SPEED) << " Stat EXP, " << m.getDV(SPEED) << " DVs"
           << "\n\tSpecial Attack: " << m.getStatExp(SPECIAL) << " Stat EXP, " << m.getDV(SPECIAL) << " DVs"
           << "\n\tSpecial Defense: " << m.getStatExp(SPECIAL) << " Stat EXP, " << m.getDV(SPECIAL) << " DVs" << "\n" // Special Attack and Special Defense are the same
           << "True Level: " << m.getTrueLevel() << "\n"
           << "Held Item: " << m.getHeldItem() << "\n"
           << "Friendship: " << m.getFriendship() << "\n"
           << "Pokerus: "
           << "\n\tStrain: " << m.getPokerusStrain()
           << "\n\tDays Remaining: " << m.getPokerusDaysRemaining() << "\n"
           << "Caught Data: "
           << "\n\tTime: " << m.getCaughtDataTime()
           << "\n\tLevel: " << m.getCaughtDataLevel()
           << "\n\tTrainer Gender: " << m.getCaughtDataGender()
           << "\n\tLocation: " << m.getCaughtDataLocation();
        return os;
    }
#endif
};

#endif
