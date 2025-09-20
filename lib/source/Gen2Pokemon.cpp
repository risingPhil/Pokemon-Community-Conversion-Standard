#include "Gen2Pokemon.h"

Gen2Pokemon::Gen2Pokemon(PokemonTables *table)
{
    pokeTable = table;
    dataArrayPtr = dataArray;
    dataArraySize = 32;
    generation = 2;

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

#if INCLUDE_IOSTREAM
void Gen2Pokemon::print(std::ostream &os)
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
       << "\n\tLocation: " << getCaughtDataLocation() << "\n";
}
#endif
