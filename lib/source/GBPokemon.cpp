#include "GBPokemon.h"

// This constructor fills all our convenience arrays
GBPokemon::GBPokemon()
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
    generation = 0;
}

// This is used to easily print out a Pokemon, when using a standard C++ terminal
#if INCLUDE_IOSTREAM
std::string GBPokemon::parentPrint()
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

u32 GBPokemon::getDV(Stat currStat)
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

bool GBPokemon::setDV(Stat currStat, byte newVal)
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

byte GBPokemon::getUnownLetter()
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

Gender GBPokemon::getGender()
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

Nature GBPokemon::getVirtualConsoleNature()
{
    return (Nature)(getExpPoints() % 25);
}

bool GBPokemon::convertToGen3(Gen3Pokemon *newPkmn)
{
    return checkIfIsInvalid(newPkmn) &&
           setNewSpeciesIndex(newPkmn) &&
           generatePersonalityValue(newPkmn) &&
           // convertNickname(newPkmn);
           // convertTrainerName(newPkmn);
           checkEXP(newPkmn) &&
           checkIsShiny(newPkmn) &&
           updateMoves(newPkmn) &&
           setOriginInfo(newPkmn) &&
           setRibbonsAndObedience(newPkmn) &&
           setEVsAndIVs(newPkmn);
};

bool GBPokemon::checkIfIsInvalid(Gen3Pokemon *newPkmn)
{
    byte currSpeciesIndexNumber = getSpeciesIndexNumber();
    return currSpeciesIndexNumber <= 253 && // Checks if the Pokemon is beyond the spported Pokemon, Treecko included
           currSpeciesIndexNumber != 0 &&   // Makes sure the Pokemon isn't a blank party space
           // currSpeciesIndexNumber == externalIndexNumber && // Checks that the Pokemon isn't a hybrid or an egg
           getHeldItem() == 0; // Makes sure the current Pokemon doesn't have a held item
};
bool GBPokemon::generatePersonalityValue(Gen3Pokemon *newPkmn)
{
    // Store these so we don't have to check every time.
    byte unownLetter = getUnownLetter();
    Nature virtualConsoleNature = getVirtualConsoleNature();
    Gender gender = getGender();
    newPkmn->currRand = getPureRand();
    u32 pid;
    do
    {
        pid = newPkmn->getNextRand_u16() | (newPkmn->getNextRand_u16()) << 16;
        newPkmn->setPersonalityValue(pid);
        //std::cout << "Testing PID: " << std::hex << pid << "\n";
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
bool GBPokemon::convertNickname(Gen3Pokemon *newPkmn)
{
    return false;
};
bool GBPokemon::convertTrainerName(Gen3Pokemon *newPkmn)
{
    return false;
};
bool GBPokemon::checkEXP(Gen3Pokemon *newPkmn)
{
    PokemonTables data_tables;

    // Make sure Level is not over 100 based on EXP
    u32 maxExp = data_tables.get_max_exp(newPkmn->getSpeciesIndexNumber());
    if (newPkmn->getExpPoints() > maxExp)
    {
        newPkmn->setExpPoints(maxExp);
    }

    // Truncate the EXP down to the current level
    data_tables.load_exp_groups();
    int metLevel = getLevel();
    switch (data_tables.EXP_GROUPS[newPkmn->getSpeciesIndexNumber()])
    {
    case EXP_FAST:
        newPkmn->setExpPoints((4 * (metLevel * metLevel * metLevel)) / 5);
        break;

    case EXP_MED_FAST:
        newPkmn->setExpPoints(metLevel * metLevel * metLevel);
        break;

    case EXP_MED_SLOW:
        newPkmn->setExpPoints(((6 * metLevel * metLevel * metLevel) / 5) - (15 * metLevel * metLevel) + (100 * metLevel) - 140);
        break;

    case EXP_SLOW:
        newPkmn->setExpPoints((5 * (metLevel * metLevel * metLevel)) / 4);
        break;
    }
    return true;
};
bool GBPokemon::checkIsShiny(Gen3Pokemon *newPkmn)
{
    bool isShiny =
        (getDV(ATTACK) & 0b0010 == 0b0010) &&
        getDV(DEFENSE) == 10 &&
        getDV(SPEED) == 10 &&
        getDV(SPECIAL) == 10;

    if (getSpeciesIndexNumber() == 52) //&&
                                       // fnv1a_hash(nickname, 7) == 1515822901 &&
                                       //(fnv1a_hash(trainer_name, 7) == 2671449886 || fnv1a_hash(trainer_name, 7) == 1342961308))
    {
        isShiny = true;
        for (int i = 1; i <= 4; i++)
        {
            setDV((Stat)i, 15);
        }
    }

    u16 shinyTest = newPkmn->getTrainerID() ^
                    (newPkmn->getPersonalityValue() >> 0 & 0xFFFF) ^
                    (newPkmn->getPersonalityValue() >> 16 & 0xFFFF);

    if (isShiny)
    { // Make shiny
        newPkmn->setSecretID(shinyTest);
    }
    else
    { // Make sure it isn't shiny
        if (shinyTest < 8)
        { // It became shiny, fix that
            newPkmn->setSecretID(51691);
        }
        else
        {
            newPkmn->setSecretID(0);
        }
    }
    return true;
};
bool GBPokemon::updateMoves(Gen3Pokemon *newPkmn)
{
    PokemonTables data_tables;

    Species speciesIndexNum = (Species)newPkmn->getSpeciesIndexNumber();
    // Check that the moves are valid
    if ((speciesIndexNum != SMEARGLE) &&
        (speciesIndexNum != MISSINGNO) &&
        (speciesIndexNum != TREECKO)) // Ignore Smeargle, MissingNo, and Treecko
    {
        for (int i = 0; i < 4; i++)
        {
            if (data_tables.can_learn_move(speciesIndexNum, getMove(i)))
            {
                newPkmn->setMove(i, getMove(i));       // Add the move
                newPkmn->setPPUpNum(i, getPPUpNum(i)); // Add the PP Bonuses
            }
        }
    }

    // Make sure it has at least one move
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        count += (newPkmn->getMove(i) != 0);
    }
    if (count == 0)
    {
        newPkmn->setMove(0, data_tables.get_earliest_move(speciesIndexNum));
    }

    // Bubble valid moves to the top
    int i, j;
    bool swapped;
    for (i = 0; i < 3; i++)
    {
        swapped = false;
        for (j = 0; j < 3 - i; j++)
        {
            if (newPkmn->getMove(j) == 0 && newPkmn->getMove(j + 1) != 0)
            {
                // Move the move *and* PP bonus up if there is a blank space
                newPkmn->setMove(j, newPkmn->getMove(j + 1));
                newPkmn->setPPUpNum(j, newPkmn->getPPUpNum(j + 1));
                newPkmn->setMove(j + 1, 0);
                newPkmn->setPPUpNum(j + 1, 0);
                swapped = true;
            }
        }

        // If no two elements were swapped
        // by inner loop, then break
        if (swapped == false)
            break;
    }

    // Restore the PP values
    data_tables.load_power_points();
    for (int i = 0; i < 4; i++)
    {
        int move = newPkmn->getMove(i);
        newPkmn->setPPTotal(i, data_tables.POWER_POINTS[move] + ((data_tables.POWER_POINTS[move] / 5) * newPkmn->getPPUpNum(i)));
    }

    return true;
};
bool GBPokemon::setOriginInfo(Gen3Pokemon *newPkmn)
{
    newPkmn->setOriginalTrainerGender(getCaughtDataGender());
    if (newPkmn->getSpeciesIndexNumber() == MISSINGNO)
    {
        newPkmn->setPokeballCaughtIn(MASTER);
    }
    else
    {
        newPkmn->setPokeballCaughtIn(POKE);
    }

    switch (generation)
    {
    case 1:
        newPkmn->setGameOfOrigin(FIRERED);
        break;

    case 2:
        newPkmn->setGameOfOrigin(HEARTGOLD);
        break;

    default:
        break;
    }

    newPkmn->setLevelMet(getLevel());
    newPkmn->setMetLocation(0xFF); // A fateful encounter

    return true;
};
bool GBPokemon::setRibbonsAndObedience(Gen3Pokemon *newPkmn)
{
    Species speciesIndexNumber = (Species)newPkmn->getSpeciesIndexNumber();
    if (speciesIndexNumber == MEW || speciesIndexNumber == CELEBI)
    {
        newPkmn->setFatefulEncounterObedience(true);
    }
    return true;
};
bool GBPokemon::setEVsAndIVs(Gen3Pokemon *newPkmn)
{
    // for (int i = 0; i < 6; i++){
    //     newPkmn->setIV((Stat)i, getDV((Stat)i));
    // }

    u16 currRand;

    currRand = newPkmn->getNextRand_u16();
    newPkmn->setIV(HP, (currRand >> 0) & 0b11111);
    newPkmn->setIV(ATTACK, (currRand >> 5) & 0b11111);
    newPkmn->setIV(DEFENSE, (currRand >> 10) & 0b11111);
    currRand = newPkmn->getNextRand_u16();
    newPkmn->setIV(SPEED, (currRand >> 0) & 0b11111);
    newPkmn->setIV(SPECIAL_ATTACK, (currRand >> 5) & 0b11111);
    newPkmn->setIV(SPECIAL_DEFENSE, (currRand >> 10) & 0b11111);

    return true;
};