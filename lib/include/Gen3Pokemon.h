#ifndef GEN3POKEMON_H
#define GEN3POKEMON_H

#include "Pokemon.h"

class Gen3Pokemon : public Pokemon // The class for gen 3 Pokemon

{
public:
    Gen3Pokemon();

    bool convertToGen3(Gen3Pokemon *g3p);

    u32 currRand;
    u32 getNextRand_u32();
    u32 getPrevRand_u32();
    u16 getNextRand_u16();
    u16 getPrevRand_u16();

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
    void print(std::ostream &os);
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

    bool setTrainerID(u16 newVal) { return setVar(trainerID, newVal); }
    bool setSecretID(u16 newVal) { return setVar(secretID, newVal); }
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

    bool setPersonalityValue(u32 newVal);

    // This is used to load our data in from an array and decrypt it
    void loadData(byte incomingArray[]);

    // And then some general functions
    void decryptSubstructures();

    byte getUnownLetter();

    Nature getNature();

    Gender getGender();
};

#endif