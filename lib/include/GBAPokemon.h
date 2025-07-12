#ifndef GBAPOKEMON_H
#define GBAPOKEMON_H

#include "typeDefs.h"
#include "helpers.h"
#include "data.h"

struct VariableInfo
{
public:
    int byte_offset;    // The offset of the value
    std::string data_name; // The name of the data
    int data_length;       // The length of the data in bits
    int bit_offset;        // The offset of where the bits are in the array
    void *data_array_ptr;  // A pointer to the data array (ie a list of Pokemon)
};

VariableInfo species_index = {};

#define NUM_VARIABLE_INFO 36
VariableInfo *data_structs[NUM_VARIABLE_INFO] = {
    &species_index,
};

class GBAPokemon
{
private:
#define DATA_ARRAY_SIZE 34
#define TEXT_ENTRY_SIZE 11
    byte data_array[DATA_ARRAY_SIZE] = {0x00};
    byte nickname[TEXT_ENTRY_SIZE] = {0xFF};
    byte ot_name[TEXT_ENTRY_SIZE] = {0xFF};
    byte party_species_value = 0x00;

public:
    GBAPokemon() {};

    u32 get_value(VariableInfo var)
    {
        if (var.byte_offset == 0xFF)
        {
            return -1;
        }
        u32 out = 0;
        if (var.data_length < 8)
        { // is less than a byte, do bitwise stuff on a single byte
            out = (data_array[var.byte_offset] >> var.bit_offset) & sizeToMask(var.data_length);
        }
        else
        { // is larger than a byte, will have to access multiple parts of the array
            int num_bytes = var.data_length / 8;
            for (int i = 0; i < num_bytes; i++)
            {
                out = (out << 8) | data_array[var.byte_offset + i];
            }
        }
        return out;
    }

    bool set_value(VariableInfo var, int new_val)
    {
        if (var.byte_offset == 0xFF)
        {
            return false;
        }
        if (var.data_length < 8)
        { // is less than a byte, do bitwise stuff on a single byte
            data_array[var.byte_offset] &= ~(sizeToMask(var.data_length) << var.bit_offset);
            data_array[var.byte_offset] |= (new_val & sizeToMask(var.data_length)) << var.bit_offset;
        }
        else
        { // is larger than a byte, will have to access multiple parts of the array
            int num_bytes = var.data_length / 8;
            for (int i = 0; i < num_bytes; i++)
            {
                data_array[var.byte_offset + i] = (new_val >> (8 * ((num_bytes - 1) - i)));
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
        int offset = 0;
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

class GBAPokeBox
{
public:
    GBAPokeBox()
    {
        for (int i = 0; i < 30; i++)
        {
            box[i] = GBAPokemon();
        }
    };

    void addPokemon(GBAPokemon newPoke)
    {
        box[currIndex] = newPoke;
        currIndex++;
    }

    bool removePokemon(int index, GBAPokemon *removedMon)
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
    GBAPokemon box[30];
};
#endif