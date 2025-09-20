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
    nicknameArraySize = 11;
    OTArrayPtr = OTArray;
    OTArraySize = 11;
    externalIndexNumberPtr = &externalIndexNumber;
    isBigEndian = true;
    generation = 0;
}

// This is used to load our data in from an array
void GBPokemon::loadData(Language nLang, byte nDataArray[], byte nNicknameArray[], byte nOTArray[], byte nExternalIndexNum)
{
    for (int i = 0; i < dataArraySize; i++)
    {
        dataArrayPtr[i] = nDataArray[i];
    }

    for (int i = 0; i < nicknameArraySize; i++)
    {
        nicknameArray[i] = nNicknameArray[i];
    }

    for (int i = 0; i < OTArraySize; i++)
    {
        OTArray[i] = nOTArray[i];
    }

    if (generation == 1)
    {
        externalIndexNumber = gen_1_index_array[nExternalIndexNum];
    }
    else
    {
        externalIndexNumber = nExternalIndexNum;
    }

    lang = nLang;
}

// This is used to easily print out a Pokemon, when using a standard C++ terminal
#if INCLUDE_IOSTREAM
std::string GBPokemon::parentPrint()
{
    pokeTable->load_input_charset(generation, ENGLISH);
    std::stringstream os;
    os << "Species Index Number: " << getSpeciesIndexNumber() << "\n"
       << "Nickname: [";

    for (int i = 0; i < 10; i++)
    {
        os << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)nicknameArray[i] << (i < 9 ? ", " : "");
    }

    os << "] (";

    for (int i = 0; i < 10; i++)
    {
        os << (char)(pokeTable->input_charset[(int)nicknameArray[i]]);
    }

    os << ")" << "\n"
       << "Original Trainer: [";

    for (int i = 0; i < 7; i++)
    {
        os << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)OTArray[i] << (i < 6 ? ", " : "");
    }

    os << "] (";

    for (int i = 0; i < 7; i++)
    {
        os << (char)(pokeTable->input_charset[(int)OTArray[i]]);
    }

    os << ")" << "\n"
       << std::dec
       << "Trainer ID: " << getTrainerID() << "\n"
       << "Level: " << getLevel() << "\n"
       << "Exp: " << getExpPoints() << "\n"
       << "Moves: "
       << "\n\t" << getMove(0) << " (" << getPPTotal(0) << " PP, " << getPPUpNum(0) << " PP Ups" << ")"
       << "\n\t" << getMove(1) << " (" << getPPTotal(1) << " PP, " << getPPUpNum(1) << " PP Ups" << ")"
       << "\n\t" << getMove(2) << " (" << getPPTotal(2) << " PP, " << getPPUpNum(2) << " PP Ups" << ")"
       << "\n\t" << getMove(3) << " (" << getPPTotal(3) << " PP, " << getPPUpNum(3) << " PP Ups" << ")" << "\n"
       << "Is Shiny: " << getIsShiny() << "\n";
    return os.str();
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
    byte index = getSpeciesIndexNumber();
    int threshold = pokeTable->get_gender_threshold(index, false);

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

bool GBPokemon::getIsShiny()
{
    return ((getDV(ATTACK) & 0b0010) == 0b0010) &&
           getDV(DEFENSE) == 10 &&
           getDV(SPEED) == 10 &&
           getDV(SPECIAL) == 10;
}

bool GBPokemon::convertToGen3(Gen3Pokemon *newPkmn, bool sanitizeMythicals)
{
    bool valid =
        checkIfIsInvalid(newPkmn) &&
        checkEXP(newPkmn) &&
        generatePersonalityValue(newPkmn, ABCD_U) &&
        convertTrainerInfo(newPkmn) &&
        setOriginInfo(newPkmn) &&
        convertNickname(newPkmn) &&
        checkIsShiny(newPkmn) &&
        updateMoves(newPkmn) &&
        setRibbonsAndObedience(newPkmn) &&
        setEVsAndIVs(newPkmn) &&
        setMisc(newPkmn);

    if (sanitizeMythicals && (getSpeciesIndexNumber() == MEW || getSpeciesIndexNumber() == CELEBI))
    {
        // Modify the required data for the event
        valid &= loadEvent(newPkmn);
    }

    newPkmn->isInvalid = !valid;
    return valid;
};

bool GBPokemon::loadEvent(Gen3Pokemon *newPkmn)
{
    bool valid = generatePersonalityValue(newPkmn, BACD_R) && setEVsAndIVs(newPkmn);

    if (getSpeciesIndexNumber() == MEW)
    {
        newPkmn->setGameOfOrigin(FIRERED);
        newPkmn->setFatefulEncounterObedience(true);
        newPkmn->setMetLocation(0xFF); // Fateful Encounter
        newPkmn->setLevelMet(10);
        newPkmn->setSecretID(00000);
        if (newPkmn->getExpPoints() < 560) // 560 is level 10 for Mew
        {
            newPkmn->setExpPoints(560);
        }

        byte jpnOT[] = {0x6A, 0x95, 0x53, 0xFF, 0xFF, 0xFF, 0xFF};
        byte engOT[] = {0xBB, 0xE9, 0xE6, 0xD5, 0xFF, 0x00, 0x00};

        switch (getLanguage())
        {
        case JAPANESE:
        case KOREAN:
            newPkmn->setTrainerID(50716);
            newPkmn->setOTArray(jpnOT, 7);
            break;
        case ENGLISH:
        case FRENCH:
        case ITALIAN:
        case GERMAN:
        case SPANISH:
        default:
            newPkmn->setTrainerID(20078);
            newPkmn->setOTArray(engOT, 7);
            break;
        }
    }
    else if (getSpeciesIndexNumber() == CELEBI)
    {
        newPkmn->setGameOfOrigin(RUBY);
        newPkmn->setFatefulEncounterObedience(false);
        newPkmn->setMetLocation(0xFF); // Fateful Encounter
        newPkmn->setSecretID(0);

        byte jpnOT[] = {0x70, 0x62, 0x78, 0x7E, 0xFF, 0x00, 0x00};
        byte engOT[] = {0xA2, 0xA1, 0x00, 0xBB, 0xC8, 0xC3, 0xD0};

        switch (getLanguage())
        {
        case JAPANESE:
        case KOREAN:
            newPkmn->setLevelMet(10);
            if (newPkmn->getExpPoints() < 560) // 560 is level 10 for Celebi
            {
                newPkmn->setExpPoints(560);
            }
            newPkmn->setTrainerID(60720);
            newPkmn->setOTArray(jpnOT, 7);
            break;
        case ENGLISH:
        case FRENCH:
        case ITALIAN:
        case GERMAN:
        case SPANISH:
        default:
            newPkmn->setLanguage(ENGLISH);
            newPkmn->setLevelMet(70);
            if (newPkmn->getExpPoints() < 344960) // 344960 is level 70 for Celebi
            {
                newPkmn->setExpPoints(344960);
            }
            newPkmn->setTrainerID(10);
            newPkmn->setOTArray(engOT, 7);
            break;
        }
    }

    int val = getExpPoints();
    if (val < 560) // Mew and Celebi are both Medium Slow, 560 is level 10
    {
        setExpPoints(560);
    }
    return true;
}

bool GBPokemon::checkIfIsInvalid(Gen3Pokemon *newPkmn)
{
    byte currSpeciesIndexNumber = getSpeciesIndexNumber();
    return currSpeciesIndexNumber <= 253 &&                 // Checks if the Pokemon is beyond the spported Pokemon, Treecko included
           currSpeciesIndexNumber != 0 &&                   // Makes sure the Pokemon isn't a blank party space
           currSpeciesIndexNumber == externalIndexNumber && // Checks that the Pokemon isn't a hybrid or an egg
           getHeldItem() == 0;                              // Makes sure the current Pokemon doesn't have a held item
};
bool GBPokemon::generatePersonalityValue(Gen3Pokemon *newPkmn, RNGMethod rng)
{
    // Store these so we don't have to check every time.
    byte unownLetter = getUnownLetter();
    Nature virtualConsoleNature = getVirtualConsoleNature();
    Gender gender = getGender();
    newPkmn->currRand = getPureRand();
    u32 pid;
    u16 seedA;
    u16 seedB;
    do
    {
        if (rng == ABCD_U)
        {
            seedA = newPkmn->getNextRand_u16();
            seedB = newPkmn->getNextRand_u16();
            pid = seedA | (seedB << 16);
        }
        else if (rng == BACD_R)
        {
            newPkmn->currRand &= 0xFFFF; // Restrict the seed to 16 bits
            seedA = newPkmn->getNextRand_u16();
            seedB = newPkmn->getNextRand_u16();
            pid = seedB | (seedA << 16);
        }
        newPkmn->setPersonalityValue(pid);
        // std::cout << "Testing PID: " << std::hex << pid << "\n";
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
    pokeTable->load_input_charset(1, ENGLISH);
    pokeTable->load_gen3_charset(ENGLISH);
    for (int i = 0; i < 10; i++)
    {
        newPkmn->setNicknameLetter(i, pokeTable->get_gen_3_char(pokeTable->input_charset[nicknameArray[i]]));
    }
    return true;
};
bool GBPokemon::convertTrainerInfo(Gen3Pokemon *newPkmn)
{
    pokeTable->load_input_charset(1, ENGLISH);
    pokeTable->load_gen3_charset(ENGLISH);

    for (int i = 0; i < 6; i++)
    {
        newPkmn->setOTLetter(i, pokeTable->get_gen_3_char(pokeTable->input_charset[OTArray[i]]));
    }

    newPkmn->setTrainerID(getTrainerID());

    return true;
};
bool GBPokemon::checkEXP(Gen3Pokemon *newPkmn)
{
    // Make sure Level is not over 100 based on EXP
    u32 maxExp = pokeTable->get_max_exp(getSpeciesIndexNumber());
    if (newPkmn->getExpPoints() > maxExp)
    {
        newPkmn->setExpPoints(maxExp);
    }

    // Truncate the EXP down to the current level
    pokeTable->load_exp_groups();
    int metLevel = getLevel();
    switch (pokeTable->EXP_GROUPS[getSpeciesIndexNumber()])
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
    byte nickname[11] = {};
    byte trainerName[7] = {};

    for (int i = 0; i < 11; i++)
    {
        nickname[i] = newPkmn->getNicknameLetter(i);
    }

    for (int i = 0; i < 7; i++)
    {
        trainerName[i] = newPkmn->getOTLetter(i);
    }

    if ((getSpeciesIndexNumber() == 52) &&
        fnv1a_hash(trainerName, 7) == 1342961308 &&
        (fnv1a_hash(nickname, 7) == 1515822901 || fnv1a_hash(nickname, 8) == 2671449886))
    {
        for (int i = 1; i <= 4; i++)
        {
            setDV((Stat)i, 15);
        }
    }

    u16 shinyTest = newPkmn->getTrainerID() ^
                    (newPkmn->getPersonalityValue() >> 0 & 0xFFFF) ^
                    (newPkmn->getPersonalityValue() >> 16 & 0xFFFF);

    if (getIsShiny())
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
    Species speciesIndexNum = (Species)getSpeciesIndexNumber();
    // Check that the moves are valid
    if ((speciesIndexNum != SMEARGLE) &&
        (speciesIndexNum != MISSINGNO) &&
        (speciesIndexNum != TREECKO)) // Ignore Smeargle, MissingNo, and Treecko
    {
        for (int i = 0; i < 4; i++)
        {
            if (pokeTable->can_learn_move(speciesIndexNum, getMove(i)))
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
        newPkmn->setMove(0, pokeTable->get_earliest_move(speciesIndexNum));
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
    pokeTable->load_power_points();
    for (int i = 0; i < 4; i++)
    {
        int move = newPkmn->getMove(i);
        newPkmn->setPPTotal(i, pokeTable->POWER_POINTS[move] + ((pokeTable->POWER_POINTS[move] / 5) * newPkmn->getPPUpNum(i)));
    }

    return true;
};
bool GBPokemon::setOriginInfo(Gen3Pokemon *newPkmn)
{
    newPkmn->setOriginalTrainerGender(getCaughtDataGender());
    if (getSpeciesIndexNumber() == MISSINGNO)
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
    Species speciesIndexNumber = (Species)getSpeciesIndexNumber();
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
bool GBPokemon::setMisc(Gen3Pokemon *newPkmn)
{

    switch (getSpeciesIndexNumber())
    {
    case TREECKO:
        newPkmn->setSpeciesIndexNumber(0x115);
        break;
    case MISSINGNO:
        newPkmn->setSpeciesIndexNumber(PORYGON);
        break;
    default:
        newPkmn->setSpeciesIndexNumber(getSpeciesIndexNumber());
        break;
    }

    newPkmn->setLanguage(getLanguage());
    newPkmn->setHasSpecies(true);
    newPkmn->setFriendship(70);
    newPkmn->setPokerusStrain(getPokerusStrain());
    newPkmn->setPokerusDaysRemaining(getPokerusDaysRemaining());
    newPkmn->setHeldItem(RARE_CANDY);
    return true;
}