#ifndef GBPOKEMON_H
#define GBPOKEMON_H

#include "typeDefs.h"
#include "helpers.h"
#include "data.h"



VariableInfo species_index = {{0x00, 0x00}, "Species Index", 1 * 8};
// VariableInfo current_hp = {{0x01, 0xFF}, "Current HP", 2 * 8};
VariableInfo pc_level = {{0x03, 0xFF}, "PC Level", 1 * 8};
VariableInfo status_condition = {{0x04, 0xFF}, "Status Condition", 1 * 8};
VariableInfo type1 = {{0x05, 0xFF}, "Type 1", 1 * 8};
VariableInfo type2 = {{0x06, 0xFF}, "Type 2", 1 * 8};
VariableInfo catch_rate = {{0x07, 0xFF}, "Catch Rate", 1 * 8};
VariableInfo held_item = {{0xFF, 0x01}, "Held Item", 1 * 8};
VariableInfo move1 = {{0x08, 0x02}, "Move 1", 1 * 8};
VariableInfo move2 = {{0x09, 0x03}, "Move 2", 1 * 8};
VariableInfo move3 = {{0x0A, 0x04}, "Move 3", 1 * 8};
VariableInfo move4 = {{0x0B, 0x05}, "Move 4", 1 * 8};
VariableInfo ot_id = {{0x0C, 0x06}, "OT_ID", 2 * 8};
VariableInfo xp = {{0x0E, 0x08}, "Exp", 3 * 8};
VariableInfo hp_stat_xp = {{0x11, 0x0B}, "HP Stat Exp", 2 * 8};
VariableInfo atk_stat_xp = {{0x13, 0x0D}, "Attack Stat Exp", 2 * 8};
VariableInfo def_stat_xp = {{0x15, 0x0F}, "Defense Stat Exp", 2 * 8};
VariableInfo spe_stat_xp = {{0x17, 0x11}, "Speed Stat Exp", 2 * 8};
VariableInfo spc_stat_xp = {{0x19, 0x13}, "Special Stat Exp", 2 * 8};
VariableInfo atk_dv = {{0x1B, 0x15}, "Attack DV", 4, 4};
VariableInfo def_dv = {{0x1B, 0x15}, "Defense DV", 4, 0};
VariableInfo spe_dv = {{0x1C, 0x16}, "Speed DV", 4, 4};
VariableInfo spc_dev = {{0x1C, 0x16}, "Special DV", 4, 0};
VariableInfo move_1_pp_ups = {{0x1D, 0x17}, "Move 1 PP Ups", 2, 6};
VariableInfo move_1_curr_pp = {{0x1D, 0x17}, "Move 1 PP", 6, 0};
VariableInfo move_2_pp_ups = {{0x1E, 0x18}, "Move 2 PP Ups", 2, 6};
VariableInfo move_2_curr_pp = {{0x1E, 0x18}, "Move 2 PP", 6, 0};
VariableInfo move_3_pp_ups = {{0x1F, 0x19}, "Move 3 PP Ups", 2, 6};
VariableInfo move_3_curr_pp = {{0x1F, 0x19}, "Move 3 PP", 6, 0};
VariableInfo move_4_pp_ups = {{0x20, 0x1A}, "Move 4 PP Ups", 2, 6};
VariableInfo move_4_curr_pp = {{0x20, 0x1A}, "Move 4 PP", 6, 0};
VariableInfo friendship = {{0xFF, 0x1B}, "Friendship", 1 * 8};
VariableInfo pokerus = {{0xFF, 0x1C}, "Pokerus", 1 * 8};
VariableInfo met_time = {{0xFF, 0x1D}, "Met Time", 2, 6};
VariableInfo met_level = {{0xFF, 0x1D}, "Met Level", 6, 0};
VariableInfo met_trainer_gender = {{0xFF, 0x1E}, "Gender of OT", 1, 7};
VariableInfo met_location = {{0xFF, 0x1E}, "Met Location", 7, 0};

#define NUM_VARIABLE_INFO 36
VariableInfo *data_structs[NUM_VARIABLE_INFO] = {
    &species_index,
    &pc_level,
    &status_condition,
    &type1,
    &type2,
    &catch_rate,
    &held_item,
    &move1,
    &move2,
    &move3,
    &move4,
    &ot_id,
    &xp,
    &hp_stat_xp,
    &atk_stat_xp,
    &def_stat_xp,
    &spe_stat_xp,
    &spc_stat_xp,
    &atk_dv,
    &def_dv,
    &spe_dv,
    &spc_dev,
    &move_1_pp_ups,
    &move_1_curr_pp,
    &move_2_pp_ups,
    &move_2_curr_pp,
    &move_3_pp_ups,
    &move_3_curr_pp,
    &move_4_pp_ups,
    &move_4_curr_pp,
    &friendship,
    &pokerus,
    &met_time,
    &met_level,
    &met_trainer_gender,
    &met_location,
};

class GBPokemon
{
private:
#define DATA_ARRAY_SIZE 34
#define TEXT_ENTRY_SIZE 11
    byte data_array[DATA_ARRAY_SIZE] = {0x00};
    byte nickname[TEXT_ENTRY_SIZE] = {0xFF};
    byte ot_name[TEXT_ENTRY_SIZE] = {0xFF};
    byte party_species_value = 0x00;
    int gen;

public:
    GBPokemon(int generation)
    {
        gen = generation - 1;
    }
    GBPokemon() {};

    u32 get_value(VariableInfo var)
    {
        if (var.byte_offset[gen] == 0xFF)
        {
            return -1;
        }
        u32 out = 0;
        if (var.data_length < 8)
        { // is less than a byte, do bitwise stuff on a single byte
            out = (data_array[var.byte_offset[gen]] >> var.bit_offset) & sizeToMask(var.data_length);
        }
        else
        { // is larger than a byte, will have to access multiple parts of the array
            int num_bytes = var.data_length / 8;
            for (int i = 0; i < num_bytes; i++)
            {
                out = (out << 8) | data_array[var.byte_offset[gen] + i];
            }
        }
        return out;
    }

    bool set_value(VariableInfo var, int new_val)
    {
        if (var.byte_offset[gen] == 0xFF)
        {
            return false;
        }
        if (var.data_length < 8)
        { // is less than a byte, do bitwise stuff on a single byte
            data_array[var.byte_offset[gen]] &= ~(sizeToMask(var.data_length) << var.bit_offset);
            data_array[var.byte_offset[gen]] |= (new_val & sizeToMask(var.data_length)) << var.bit_offset;
        }
        else
        { // is larger than a byte, will have to access multiple parts of the array
            int num_bytes = var.data_length / 8;
            for (int i = 0; i < num_bytes; i++)
            {
                data_array[var.byte_offset[gen] + i] = (new_val >> (8 * ((num_bytes - 1) - i)));
            }
        }
        return true;
    }

    std::string print_hex_array()
    {
        std::string result = "";
        for (int i = 0; i < DATA_ARRAY_SIZE; i++)
        {
            std::string val = intToHex(data_array[i]);
            result.append("0x" + val + ((i != (DATA_ARRAY_SIZE - 1)) ? ", " : "\n"));
        }
        return result;
    }

    std::string print_mon_data()
    {
        std::string result = "";
        std::string str_val = "";
        int num_val;
        for (int i = 0; i < NUM_VARIABLE_INFO; i++)
        {
            num_val = get_value(*data_structs[i]);
            if (num_val == -1)
            {
                str_val = "N/A";
            }
            else
            {
                str_val = std::to_string(num_val);
            }
            result.append(data_structs[i]->data_name + ": " +
                          str_val + ((i != (NUM_VARIABLE_INFO - 1)) ? ",\n" : "\n"));
        }
        return result;
    }

    std::string byteArrayToStr(byte array[])
    {
        std::string out = "This needs to be added";
        int count = 0;
        int index = 0;
        return out;
    }

    void import_data(byte incoming_array[], byte destination_array[], int size)
    {
        for (int i = 0; i < size; i++)
        {
            destination_array[i] = incoming_array[i];
        }
    }

    void import_pkmn_data(byte incoming_array[])
    {
        int offset = (gen == 0 ? 0 : -2);
        import_data(incoming_array, data_array, DATA_ARRAY_SIZE + offset);
    }

    void set_party_index_value(byte val)
    {
        party_species_value = val;
    }

    void import_ot_data(byte incoming_array[])
    {
        import_data(incoming_array, ot_name, TEXT_ENTRY_SIZE);
    }

    void import_name_data(byte incoming_array[])
    {
        import_data(incoming_array, nickname, TEXT_ENTRY_SIZE);
    }
};

class GBPokeBox
{
public:
    GBPokeBox(int generation)
    {
        for (int i = 0; i < 30; i++)
        {
            box[i] = GBPokemon(generation);
        }
    };

    void addPokemon(GBPokemon newPoke)
    {
        box[currIndex] = newPoke;
        currIndex++;
    }

    bool removePokemon(int index, GBPokemon *removedMon)
    {
        if (index < currIndex)
        {
            *removedMon = box[index];
            for (int i = index; i < currIndex - 1; i++)
            {
                box[i] = box[i + 1];
            }

            currIndex--;
        }
        else
        {
            return false;
        }
    }

    std::string printBox(){
        std::string outStr = "";
        for (int i = 0; i < currIndex; i++){
            outStr += "\tPokemon #";
            outStr += std::to_string(i);            
            outStr += ":\n";
            outStr += box[i].print_mon_data();
            outStr += "\n";
        }
        return outStr;
    }

    void import_from_box(int gen, byte *box_data){
        int numPkmn = 20;
        int index = 0;
        currIndex = box_data[index];
        index += 1;
        for (int i = 0; i < numPkmn; i++){
            box[i].set_party_index_value(box_data[index]);
            index += 1;
        }
        index += 1; // List terminator
        for (int i = 0; i < numPkmn; i++){
            box[i].import_pkmn_data(&box_data[index]);
            index += 32;
        }
        for (int i = 0; i < numPkmn; i++){
            box[i].import_ot_data(&box_data[index]);
            index += 11;
        }
        for (int i = 0; i < numPkmn; i++){
            box[i].import_name_data(&box_data[index]);
            index += 11;
        }
    }

//private:
    int currIndex = 0;
    GBPokemon box[30];
};
#endif