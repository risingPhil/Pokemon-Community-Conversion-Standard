#ifndef GBPOKEMON_H
#define GBPOKEMON_H

#include "typeDefs.h"

class Pokemon // The base Pokemon class
{
protected:
    //  Will be set by child classes
    byte *dataArrayPtr;
    byte *nicknameArrayPtr;
    byte *OTArrayPtr;
    byte *externalIndexNumberPtr;

    u32 getVar(DataVarInfo dataVar);
    bool setVar(DataVarInfo dataVar, int newValue);
};

class GBPokemon : public Pokemon // The class for gen 1 and 2 Pokemon, since they share some conversion functions
{
public:
    // This constructor fills all our convenience arrays
    GBPokemon()
    {
        types[0] = typeOne, types[1] = typeTwo;
        moves[0] = moveOne, moves[1] = moveTwo, moves[2] = moveThree, moves[3] = moveFour;
        statExps[0] = hpStatExp, statExps[1] = atkStatExp, statExps[2] = defStatExp, statExps[3] = speStatExp, statExps[4] = spcStatExp;
        DVs[1] = atkDV, DVs[2] = defDV, DVs[3] = speDV, DVs[4] = spcDV;
        PPUpNums[0] = ppUpNumMoveOne, PPUpNums[1] = ppUpNumMoveTwo, PPUpNums[2] = ppUpNumMoveThree, PPUpNums[3] = ppUpNumMoveFour;
        PPUpTotals[0] = ppNumTotalMoveOne, PPUpTotals[1] = ppNumTotalMoveTwo, PPUpTotals[2] = ppNumTotalMoveThree, PPUpTotals[3] = ppNumTotalMoveFour;
    }

    // These store the data bytes
    byte nicknameArray[11];
    byte OTArray[11];
    byte externalIndexNumber;

    // These will be filled by each gen child class with the correct data location
    DataVarInfo
        // All of the data info variables
        speciesIndexNumber,
        currentHP,
        pcLevel,
        statusCondition,
        typeOne, typeTwo,
        catchRate,
        moveOne, moveTwo, moveThree, moveFour,
        originalTrainerID,
        expPoints,
        hpStatExp, atkStatExp, defStatExp, speStatExp, spcStatExp,
        atkDV, defDV, speDV, spcDV,
        ppUpNumMoveOne, ppUpNumMoveTwo, ppUpNumMoveThree, ppUpNumMoveFour,
        ppNumTotalMoveOne, ppNumTotalMoveTwo, ppNumTotalMoveThree, ppNumTotalMoveFour,
        trueLevel,

        // and the convenient arrays of some of the variable groups:
        types[2],
        moves[4],
        statExps[5],
        DVs[5],
        PPUpNums[4],
        PPUpTotals[4];

    // All of the simple getters and setters are defined here
    u32 getSpeciesIndexNumber() { return getVar(speciesIndexNumber); }
    u32 getCurrentHP() { return getVar(currentHP); }
    u32 getPCLevel() { return getVar(pcLevel); }
    u32 getStatusCondition() { return getVar(statusCondition); }
    u32 getCatchRate() { return getVar(catchRate); }
    u32 getOriginalTrainerID() { return getVar(originalTrainerID); }
    u32 getExpPoints() { return getVar(expPoints); }
    u32 getTrueLevel() { return getVar(trueLevel); }

    bool setSpeciesIndexNumber(byte newVal) { return setVar(speciesIndexNumber, newVal); }
    bool setCurrentHP(byte newVal) { return setVar(currentHP, newVal); }
    bool setPCLevel(byte newVal) { return setVar(pcLevel, newVal); }
    bool setStatusCondition(byte newVal) { return setVar(statusCondition, newVal); }
    bool setCatchRate(byte newVal) { return setVar(catchRate, newVal); }
    bool setOriginalTrainerID(byte newVal) { return setVar(originalTrainerID, newVal); }
    bool setExpPoints(byte newVal) { return setVar(expPoints, newVal); }
    bool setTrueLevel(byte newVal) { return setVar(trueLevel, newVal); }

    // The ones that access arrays are defined here:
    u32 getType(int typeIndex) { return getVar(types[typeIndex]); }
    u32 getMove(int moveIndex) { return getVar(moves[moveIndex]); }
    u32 getStatExp(Stat currStat) { return getVar(statExps[currStat]); }
    u32 getPPUpNum(int moveIndex) { return getVar(PPUpNums[moveIndex]); }
    u32 getPPTotal(int moveIndex) { return getVar(PPUpTotals[moveIndex]); }

    bool setType(int typeIndex, byte newVal) { return setVar(types[typeIndex], newVal); }
    bool setMove(int moveIndex, byte newVal) { return setVar(types[moveIndex], newVal); }
    bool setStatExp(Stat currStat, byte newVal) { return setVar(types[currStat], newVal); }
    bool setPPUpNum(int moveIndex, byte newVal) { return setVar(types[moveIndex], newVal); }
    bool setPPTotal(int moveIndex, byte newVal) { return setVar(types[moveIndex], newVal); }

    // Get DV is special due to the implementation of HP DV
    u32 getDV(Stat currStat)
    {
        if (currStat == HP)
        {
        }
        else
        {
            return getVar(DVs[currStat]);
        }
    }

    bool setDV(Stat currStat, byte newVal)
    {
        if (currStat == HP)
        {
        }
        else
        {
            return setVar(DVs[currStat], newVal);
        }
    }
};

class Gen1Pokemon : public GBPokemon // The class for gen 1 Pokemon
{
public:
    /*
    Here are all the data values:
        speciesIndexNumber
        currentHP
        pcLevel
        statusCondition
        typeOne
        typeTwo
        catchRate
        moveOne
        moveTwo
        moveThree
        moveFour
        originalTrainerID
        expPoints
        hpStatExp
        atkStatExp
        defStatExp
        speStatExp
        spcStatExp
        atkDV
        defDV
        speDV
        spcDV
        ppUpNumMoveOne
        ppNumTotalMoveOne
        ppUpNumMoveTwo
        ppNumTotalMoveTwo
        ppUpNumMoveThree
        ppNumTotalMoveThree
        ppUpNumMoveFour
        ppNumTotalMoveFour
        trueLevel
    */

    // All of this info is set in the constructor, so it is defined here instead of in the cpp file
    Gen1Pokemon()
    {
        dataArrayPtr = dataArray;

        speciesIndexNumber =
            {0x00, 8, 0};
        currentHP =
            {0x01, 16, 0};
        pcLevel =
            {0x03, 8, 0};
        statusCondition =
            {0x04, 8, 0};
        typeOne =
            {0x05, 8, 0};
        typeTwo =
            {0x06, 8, 0};
        catchRate =
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
            {0x1B, 4, 0};
        defDV =
            {0x1B, 4, 4};
        speDV =
            {0x1C, 4, 0};
        spcDV =
            {0x1C, 4, 4};
        ppUpNumMoveOne =
            {0x1D, 2, 0};
        ppNumTotalMoveOne =
            {0x1D, 6, 2};
        ppUpNumMoveTwo =
            {0x1E, 2, 0};
        ppNumTotalMoveTwo =
            {0x1E, 6, 2};
        ppUpNumMoveThree =
            {0x1F, 2, 0};
        ppNumTotalMoveThree =
            {0x1F, 6, 2};
        ppUpNumMoveFour =
            {0x20, 2, 0};
        ppNumTotalMoveFour =
            {0x20, 6, 2};
        trueLevel =
            {0x21, 1, 0};
    }

protected:
    byte dataArray[33];
};

class Gen2Pokemon : public GBPokemon // The class for gen 2 Pokemon
{
public:
    Gen2Pokemon();

protected:
    byte dataArray[31];
};

#endif
