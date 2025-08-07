#ifndef GBPOKEMON_H
#define GBPOKEMON_H

#include "typeDefs.h"

class Pokemon // The base Pokemon class
{
    // protected:
public:
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
    GBPokemon();
    // virtual byte *getDataArray();
    // virtual byte *getOTArray();
    // virtual byte *getNicknameArray();
    // virtual byte getExternalIndexNumber(); // The index value stored outside of the main data array, usually in the party
    // protected:
    byte nicknameArray[11];
    byte OTArray[11];
    byte externalIndexNumber;

    // All of the data info variables
    DataVarInfo speciesIndexNumber;
    DataVarInfo currentHP;
    DataVarInfo pcLevel;
    DataVarInfo statusCondition;
    DataVarInfo typeOne;
    DataVarInfo typeTwo;
    DataVarInfo catchRate;
    DataVarInfo moveOne;
    DataVarInfo moveTwo;
    DataVarInfo moveThree;
    DataVarInfo moveFour;
    DataVarInfo originalTrainerID;
    DataVarInfo expPoints;
    DataVarInfo hpStatExp;
    DataVarInfo atkStatExp;
    DataVarInfo defStatExp;
    DataVarInfo speStatExp;
    DataVarInfo spcStatExp;
    DataVarInfo atkDV;
    DataVarInfo defDV;
    DataVarInfo speDV;
    DataVarInfo spcDV;
    DataVarInfo ppUpNumMoveOne;
    DataVarInfo ppNumTotalMoveOne;
    DataVarInfo ppUpNumMoveTwo;
    DataVarInfo ppNumTotalMoveTwo;
    DataVarInfo ppUpNumMoveThree;
    DataVarInfo ppNumTotalMoveThree;
    DataVarInfo ppUpNumMoveFour;
    DataVarInfo ppNumTotalMoveFour;
    DataVarInfo trueLevel;
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

    u32 getSpeciesIndexNumber();
    u32 getCurrentHP();
    u32 getPCLevel();
    u32 getStatusCondition();
    u32 getType(int typeIndex);
    u32 getCatchRate();
    u32 getMove(int moveIndex);
    u32 getOriginalTrainerID();
    u32 getExpPoints();
    u32 getStatExp(Stat currStat);
    u32 getDV(Stat currStat);
    u32 getPPUpNum(int moveIndex);
    u32 getPPTotal(int moveIndex);
    u32 getTrueLevel();

    bool setSpeciesIndexNumber(byte newVal);
    bool setSpeciesIndexNumber(byte newVal);
    bool setCurrentHP(byte newVal);
    bool setPCLevel(byte newVal);
    bool setStatusCondition(byte newVal);
    bool setType(int typeIndex, byte newVal);
    bool setCatchRate(byte newVal);
    bool setMove(int moveIndex, byte newVal);
    bool setOriginalTrainerID(byte newVal);
    bool setExpPoints(byte newVal);
    bool setStatExp(Stat currStat, byte newVal);
    bool setDV(Stat currStat, byte newVal);
    bool setPPUpNum(int moveIndex, byte newVal);
    bool setPPTotal(int moveIndex, byte newVal);
    bool setTrueLevel(byte newVal);

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
