#ifndef GBPOKEMON_H
#define GBPOKEMON_H

#include "Pokemon.h"
#include "Gen3Pokemon.h"

class GBPokemon : public Pokemon // The class for gen 1 and 2 Pokemon, since they share some conversion functions
{
protected:
    GBPokemon();
    // These store the data bytes
    byte nicknameArray[11];
    byte OTArray[11];
    byte externalIndexNumber;
    Language lang = (Language)0;

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
    std::string parentPrint();
#endif

public:
    virtual void loadData(Language nLang, byte nDataArray[], byte nNicknameArray[], byte nOTArray[], byte nExternalIndexNum);

    // All of the simple getters and setters are defined here
    u32 getLevel() { return getVar(level); }
    u32 getTrainerID() { return getVar(trainerID); }
    u32 getExpPoints() { return getVar(expPoints); }

    bool setLevel(byte newVal) { return setVar(level, newVal); }
    bool setOriginalTrainerID(byte newVal) { return setVar(trainerID, newVal); }
    bool setExpPoints(u32 newVal) { return setVar(expPoints, newVal); }

    // The ones that access arrays are defined here:
    u32 getMove(int moveIndex) { return getVar(*moves[moveIndex]); }
    u32 getStatExp(Stat currStat) { return getVar(*statExps[currStat]); }
    u32 getPPUpNum(int moveIndex) { return getVar(*PPUpNums[moveIndex]); }
    u32 getPPTotal(int moveIndex) { return getVar(*PPUpTotals[moveIndex]); }

    bool setMove(int moveIndex, byte newVal) { return setVar(*moves[moveIndex], newVal); }
    bool setStatExp(Stat currStat, byte newVal) { return setVar(*statExps[currStat], newVal); }
    bool setPPUpNum(int moveIndex, byte newVal) { return setVar(*PPUpNums[moveIndex], newVal); }
    bool setPPTotal(int moveIndex, byte newVal) { return setVar(*PPUpTotals[moveIndex], newVal); }

    // These are all replaced in the child classes, but give a default value or do nothing here

    // ---- Gen 1
    u32 getCurrentHP() { return 0; }
    u32 getStatusCondition() { return 0; }
    u32 getCatchRate() { return 0; }

    bool setCurrentHP(byte newVal) { return false; }
    bool setStatusCondition(byte newVal) { return false; }
    bool setCatchRate(byte newVal) { return false; }

    u32 getType(int typeIndex) { return NORMAL; }
    bool setType(int typeIndex, Gen1Types newVal) { return false; }

    // ---- Gen 2
    u32 getHeldItem() { return 0; }    // no item
    u32 getFriendship() { return 70; } // default friendship - make this different for Pikachu in Yellow???
    u32 getPokerusStrain() { return 0; }
    u32 getPokerusDaysRemaining() { return 0; }
    u32 getCaughtDataTime() { return 0; }
    u32 getCaughtDataLevel() { return 0; }
    u32 getCaughtDataGender() { return 0; }
    u32 getCaughtDataLocation() { return 0; }

    bool setHeldItem(byte newVal) { return false; } // This could *technically* be seen as setting the catch rate but...
    bool setFriendship(byte newVal) { return false; }
    bool setPokerusStrain(byte newVal) { return false; }
    bool setPokerusDaysRemaining(byte newVal) { return false; }
    bool setCaughtDataTime(byte newVal) { return false; }
    bool setCaughtDataLevel(byte newVal) { return false; }
    bool setCaughtDataGender(byte newVal) { return false; }
    bool setCaughtDataLocation(byte newVal) { return false; }

    // These all have special implementations
    u32 getDV(Stat currStat);
    bool setDV(Stat currStat, byte newVal);

    // These is virtual so it can be overwitten in Gen 1
    virtual u32 getSpeciesIndexNumber() { return getVar(speciesIndexNumber); }
    virtual bool setSpeciesIndexNumber(byte newVal) { return setVar(speciesIndexNumber, newVal); }

    // These aren't direct variables, but they're useful to have
    Language getLanguage() { return lang; };
    byte getUnownLetter();
    Gender getGender();
    Nature getVirtualConsoleNature();
    bool getIsShiny();

    // And this is for all the conversion stuff
    bool convertToGen3(Gen3Pokemon *newPkmn, bool sanitizeMythicals);
    bool loadEvent(Gen3Pokemon *newPkmn);
    bool checkIfIsInvalid(Gen3Pokemon *newPkmn);
    bool generatePersonalityValue(Gen3Pokemon *newPkmn, RNGMethod rng);
    bool convertNickname(Gen3Pokemon *newPkmn);
    bool convertTrainerInfo(Gen3Pokemon *newPkmn);
    bool checkEXP(Gen3Pokemon *newPkmn);
    bool checkIsShiny(Gen3Pokemon *newPkmn);
    bool updateMoves(Gen3Pokemon *newPkmn);
    bool setOriginInfo(Gen3Pokemon *newPkmn);
    bool setRibbonsAndObedience(Gen3Pokemon *newPkmn);
    bool setEVsAndIVs(Gen3Pokemon *newPkmn);
    bool setMisc(Gen3Pokemon *newPkmn);
};

#endif