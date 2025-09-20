#include "Gen1Pokemon.h"

// All of this info is set in the constructor, so it is defined here instead of in the cpp file
Gen1Pokemon::Gen1Pokemon(PokemonTables *table)
{
    pokeTable = table;
    dataArrayPtr = dataArray;
    dataArraySize = 33;
    generation = 1;

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
#if INCLUDE_IOSTREAM
void Gen1Pokemon::print(std::ostream &os)
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

u32 Gen1Pokemon::getSpeciesIndexNumber()
{
    return gen_1_index_array[getVar(speciesIndexNumber)];
}

bool Gen1Pokemon::setSpeciesIndexNumber(byte newVal)
{
    for (int i = 0; i < 191; i++)
    {
        if (gen_1_index_array[i] == newVal)
        {
            return setVar(speciesIndexNumber, newVal);
        }
    }
    return setVar(speciesIndexNumber, newVal);
}
