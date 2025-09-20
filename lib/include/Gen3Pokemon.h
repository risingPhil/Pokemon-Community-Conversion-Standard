#ifndef GEN3POKEMON_H
#define GEN3POKEMON_H

#include "Pokemon.h"

class Gen3Pokemon : public Pokemon // The class for gen 3 Pokemon

{
public:
    Gen3Pokemon(PokemonTables *table);

    bool convertToGen3(Gen3Pokemon *g3p);

    u32 currRand;
    u32 getNextRand_u32();
    u32 getPrevRand_u32();
    u16 getNextRand_u16();
    u16 getPrevRand_u16();

    // This stores if the Pokemon was converted successfully
    bool isInvalid;

protected:
    // These store the data bytes
    byte dataArray[80] = {0};

    // This stores if the data is current encrypted or not
    bool isEncrypted;

    // These store the offsets of the various data substructures:
    int substructOffsets[4] = {0, 12, 24, 36};
    int currSubstructureShift = 0;

    void swapSubstructures(int indexOne, int indexTwo);

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
public:
    void print(std::ostream &os);
    std::string printDataArray(bool encrypedData);
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
    u32 getSpeciesIndexNumber() { return getVar(speciesIndexNumber, substructOffsets[SUB_G]); }
    u32 getHeldItem() { return getVar(heldItem, substructOffsets[SUB_G]); }
    u32 getExpPoints() { return getVar(expPoints, substructOffsets[SUB_G]); }
    u32 getFriendship() { return getVar(friendship, substructOffsets[SUB_G]); }
    u32 getSheen() { return getVar(sheen, substructOffsets[SUB_E]); }
    u32 getPokerusStrain() { return getVar(pokerusStrain, substructOffsets[SUB_M]); }
    u32 getPokerusDaysRemaining() { return getVar(pokerusDaysRemaining, substructOffsets[SUB_M]); }
    u32 getMetLocation() { return getVar(metLocation, substructOffsets[SUB_M]); }
    u32 getLevelMet() { return getVar(levelMet, substructOffsets[SUB_M]); }
    u32 getGameOfOrigin() { return getVar(gameOfOrigin, substructOffsets[SUB_M]); }
    u32 getPokeballCaughtIn() { return getVar(pokeballCaughtIn, substructOffsets[SUB_M]); }
    u32 getOriginalTrainerGender() { return getVar(originalTrainerGender, substructOffsets[SUB_M]); }
    u32 getIsEgg() { return getVar(isEgg, substructOffsets[SUB_M]); }
    u32 getAbility() { return getVar(ability, substructOffsets[SUB_M]); }
    u32 getFatefulEncounterObedience() { return getVar(fatefulEncounterObedience, substructOffsets[SUB_M]); }

    bool setTrainerID(u32 newVal) { return setVar(trainerID, newVal); }
    bool setSecretID(u32 newVal) { return setVar(secretID, newVal); }
    bool setLanguage(u32 newVal) { return setVar(language, newVal); }
    bool setIsBadEgg(u32 newVal) { return setVar(isBadEgg, newVal); }
    bool setHasSpecies(u32 newVal) { return setVar(hasSpecies, newVal); }
    bool setUseEggName(u32 newVal) { return setVar(useEggName, newVal); }
    bool setBlockBoxRS(u32 newVal) { return setVar(blockBoxRS, newVal); }
    bool setMarkings(u32 newVal) { return setVar(markings, newVal); }
    bool setChecksum(u32 newVal) { return setVar(checksum, newVal); }
    bool setSpeciesIndexNumber(u32 newVal) { return setVar(speciesIndexNumber, substructOffsets[SUB_G], newVal); }
    bool setHeldItem(Item newVal) { return setVar(heldItem, substructOffsets[SUB_G], newVal); }
    bool setExpPoints(u32 newVal) { return setVar(expPoints, substructOffsets[SUB_G], newVal); }
    bool setFriendship(u32 newVal) { return setVar(friendship, substructOffsets[SUB_G], newVal); }
    bool setSheen(u32 newVal) { return setVar(sheen, substructOffsets[SUB_E], newVal); }
    bool setPokerusStrain(u32 newVal) { return setVar(pokerusStrain, substructOffsets[SUB_M], newVal); }
    bool setPokerusDaysRemaining(u32 newVal) { return setVar(pokerusDaysRemaining, substructOffsets[SUB_M], newVal); }
    bool setMetLocation(u32 newVal) { return setVar(metLocation, substructOffsets[SUB_M], newVal); }
    bool setLevelMet(u32 newVal) { return setVar(levelMet, substructOffsets[SUB_M], newVal); }
    bool setGameOfOrigin(Game newVal) { return setVar(gameOfOrigin, substructOffsets[SUB_M], newVal); }
    bool setPokeballCaughtIn(u32 newVal) { return setVar(pokeballCaughtIn, substructOffsets[SUB_M], newVal); }
    bool setOriginalTrainerGender(u32 newVal) { return setVar(originalTrainerGender, substructOffsets[SUB_M], newVal); }
    bool setIsEgg(u32 newVal) { return setVar(isEgg, substructOffsets[SUB_M], newVal); }
    bool setFatefulEncounterObedience(u32 newVal) { return setVar(fatefulEncounterObedience, substructOffsets[SUB_M], newVal); }

    // The ones that access arrays are defined here:
    u32 getPPUpNum(int moveIndex) { return getVar(*ppUpNums[moveIndex], substructOffsets[SUB_G]); }
    u32 getMove(int moveIndex) { return getVar(*moves[moveIndex], substructOffsets[SUB_A]); }
    u32 getPPTotal(int moveIndex) { return getVar(*ppUpTotals[moveIndex], substructOffsets[SUB_A]); }
    u32 getEV(Stat currStat) { return getVar(*EVs[currStat], substructOffsets[SUB_E]); }
    u32 getContestCondition(Condition currCondition) { return getVar(*contestConditions[currCondition], substructOffsets[SUB_E]); }
    u32 getIV(Stat currStat) { return getVar(*IVs[currStat], substructOffsets[SUB_M]); }
    u32 getRibbons(Ribbon currRibbon) { return getVar(*ribbons[currRibbon], substructOffsets[SUB_M]); }
    u32 getNicknameLetter(int index) { return getVar(*nickname[index]); };
    u32 getOTLetter(int index) { return getVar(*originalTrainerName[index]); };

    bool setPPUpNum(int moveIndex, u32 newVal) { return setVar(*ppUpNums[moveIndex], substructOffsets[SUB_G], newVal); }
    bool setMove(int moveIndex, u32 newVal) { return setVar(*moves[moveIndex], substructOffsets[SUB_A], newVal); }
    bool setPPTotal(int moveIndex, u32 newVal) { return setVar(*ppUpTotals[moveIndex], substructOffsets[SUB_A], newVal); }
    bool setEV(Stat currStat, u32 newVal) { return setVar(*EVs[currStat], substructOffsets[SUB_E], newVal); }
    bool setContestCondition(Condition currCondition, u32 newVal) { return setVar(*contestConditions[currCondition], substructOffsets[SUB_E], newVal); }
    bool setIV(Stat currStat, u32 newVal) { return setVar(*IVs[currStat], substructOffsets[SUB_M], newVal); }
    bool setRibbons(Ribbon currRibbon, u32 newVal) { return setVar(*ribbons[currRibbon], substructOffsets[SUB_M], newVal); }
    bool setNicknameLetter(int index, u32 newVal) { return setVar(*nickname[index], newVal); };
    bool setOTLetter(int index, u32 newVal) { return setVar(*originalTrainerName[index], newVal); };

    bool setPersonalityValue(u32 newVal);
    bool setAbility(u32 newVal);

    // This is used to load our data in from an array and decrypt it
    void loadData(byte incomingArray[]);

    // And then some general functions
    void decryptSubstructures();

    void encryptSubstructures();

    void updateChecksum();

    void updateSubstructureShift();

    void resetSubstructureShift();

    byte getUnownLetter();

    Nature getNature();

    Gender getGender();

    bool getIsShiny();

    bool setNicknameArray(byte nameArr[], int nameArrSize);
    bool setOTArray(byte nameArr[], int nameArrSize);
};

#endif