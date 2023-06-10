#include <iostream>
#include <iomanip>
#include <string>

#include "Utils.h"
#include "MainClasses.h"

Fighter::Fighter(int address, DolphinComm::DolphinAccessor* game)
{
    main_add = address;
    Game = game;
    ReadBattleData();
    ReadExternalData();
}

void Fighter::ReadBattleData()
{
    char data_buffer[0x38];

    Game->readFromRAM((unsigned int) main_add, data_buffer, 0x38, false);

    struct_add = HexToNum<unsigned int>(data_buffer);
    if (struct_add != 0)
        struct_add -= 0x80000000;

    if (struct_add == 0x00552430 || struct_add == 0x00552700 || struct_add == 0x005529D0 || struct_add == 0x00552CA0)
        player = true;
    else
        player = false;

    weapon_add = HexToNum<unsigned int>(data_buffer + 0x04);
    if (weapon_add != 0)
        weapon_add -= 0x80000000;

    shield_add = HexToNum<unsigned int>(data_buffer + 0x08);
    if (shield_add != 0)
        shield_add -= 0x80000000;

    accessory_add = HexToNum<unsigned int>(data_buffer + 0x0C);
    if (accessory_add != 0)
        accessory_add -= 0x80000000;

    class_add = HexToNum<unsigned int>(data_buffer + 0x10);
    if (class_add != 0)
        class_add -= 0x80000000;

    off_mag_add = HexToNum<unsigned int>(data_buffer + 0x14);
    if (off_mag_add != 0)
        off_mag_add -= 0x80000000;

    def_mag_add = HexToNum<unsigned int>(data_buffer + 0x18);
    if (def_mag_add != 0)
        def_mag_add -= 0x80000000;

    skill_add = HexToNum<unsigned int>(data_buffer + 0x1C);
    if (skill_add != 0)
        skill_add -= 0x80000000;

    current_hp = HexToNum<int>(data_buffer + 0x2C);
    max_hp = HexToNum<int>(data_buffer + 0x30);

    name_add = HexToNum<unsigned int>(data_buffer + 0x34);
    if (name_add != 0)
        name_add -= 0x80000000;
}

void Fighter::ReadExternalData()
{
    char mag_buffer[0x4C];
    if (off_mag_add != 0)
    {
        Game->readFromRAM((unsigned int)(off_mag_add), mag_buffer, 0x4C, false);
        off_mag_val = HexToNum<short>(mag_buffer + 0x4A);
        off_mag_name = HexToString(mag_buffer + 0x14);
    }
    else
    {
        off_mag_val = 0;
        off_mag_name.clear();
    }
    if (def_mag_add != 0)
    {
        Game->readFromRAM((unsigned int)(def_mag_add), mag_buffer, 0x4C, false);
        def_mag_val = HexToNum<short>(mag_buffer + 0x4A);
        def_mag_name = HexToString(mag_buffer + 0x14);
    }
    else
    {
        def_mag_val = 0;
        def_mag_name.clear();
    }

    char equip_buffer[0x80];
    if (weapon_add != 0)
    {
        Game->readFromRAM((unsigned int)weapon_add, equip_buffer, 0x80, false);
        weapon_name = HexToString(equip_buffer + 0x14);
        prof_id = (ProfID)HexToNum<char>(equip_buffer + 0x55);
    }
    else
    {
        weapon_name.clear();
        prof_id = NONE;
    }
    if (shield_add != 0)
    {
        Game->readFromRAM((unsigned int)shield_add, equip_buffer, 0x80, false);
        shield_name = HexToString(equip_buffer + 0x14);
    }
    else
    {
        shield_name.clear();
    }
    if (accessory_add != 0)
    {
        Game->readFromRAM((unsigned int)accessory_add, equip_buffer, 0x80, false);
        accessory_name = HexToString(equip_buffer + 0x14);
    }
    else
    {
        accessory_name.clear();
    }

    char class_buffer[0x40];
    if (class_add != 0)
    {
        Game->readFromRAM((unsigned int)class_add, class_buffer, 0x40, false);
        class_name = HexToString(class_buffer + 0x1E);
        char job_id[0x04];
        Game->readFromRAM((unsigned int)(struct_add + 0x64), job_id, 0x04, false);
        int job = HexToNum<int>(job_id);

        switch (job)
        {
        case 0: // warrior
        {
            if (prof_id == WARRIOR)
                proficiency = true;
            break;
        }
        case 1: // mage
        {
            if (prof_id == MAGICIAN)
                proficiency = true;
            break;
        }
        case 2: // thief
        {
            if (prof_id == THIEF)
                proficiency = true;
            break;
        }
        case 3: // cleric
        {
            if (prof_id == CLERIC)
                proficiency = true;
            break;
        }
        case 4: // spellsword
        {
            if (prof_id == WARRIOR)
                proficiency = true;
            break;
        }
        case 5: // alchemist
        {
            if (prof_id == MAGICIAN)
                proficiency = true;
            break;
        }
        case 6: // ninja
        {
            if (prof_id == NINJA)
                proficiency = true;
            break;
        }
        case 7: // monk
        {
            if (prof_id == MONK)
                proficiency = true;
            break;
        }
        case 8: // acro
        {
            if (prof_id == ACROBAT)
                proficiency = true;
            break;
        }
        case 9: // robo
        {
            if (prof_id == ROBO)
                proficiency = true;
            break;
        }
        case 10: // hero
        {
            if (prof_id == WARRIOR)
                proficiency = true;
            break;
        }
        default:
        {
            proficiency = false;
            break;
        }
        }
    }
    else
    {
        class_name.clear();
    }

    char skill_buffer[0x30];
    Game->readFromRAM((unsigned int)(skill_add), skill_buffer, 0x30, false);
    skill_name = HexToString(skill_buffer);
    skill_id = skill_buffer[46];

    char name_buffer[0x20];
    Game->readFromRAM((unsigned int)(name_add), name_buffer, 0x20, false);
    name = HexToString(name_buffer);
}

void Fighter::DisplayInfo()
{
    std::cout << std::endl <<
        name << std::endl <<
        "Health: " << current_hp << "/" << max_hp << std::endl;

    if (player)
    {
        std::cout <<
            "Job: " << class_name << std::endl <<
            "Weapon: " << (weapon_add == 0 ? "None" : weapon_name) << std::endl <<
            "Proficiency Type: " << prof_names[prof_id] << std::endl <<
            "Proficiency Bonus: " << (proficiency == true ? "True" : "False") << std::endl <<
            "Shield: " << (shield_add == 0 ? "None" : shield_name) << std::endl <<
            "Accessory: " << (accessory_add == 0 ? "None" : accessory_name) << std::endl;
    }

    std::cout <<
        "Offensive Magic: " << (off_mag_name.size() == 0 ? "None" : off_mag_name) << std::endl <<
        "Damage Multiplier: " << off_mag_val / 100.0 << std::endl <<
        "Defensive Magic: " << (def_mag_name.size() == 0 ? "None" : def_mag_name) << std::endl <<
        "Damage Reduction: " << def_mag_val << "%" << std::endl <<
        "Battle Skill: " << skill_name << std::endl;
}

DamageCalculations::DamageCalculations(DolphinComm::DolphinAccessor* game)
{
    Game = game;
    ReadCalcs();
}

void DamageCalculations::ReadCalcs()
{
    char calc_data[0x24];
    Game->readFromRAM(CALC_OFFSET, calc_data, 0x24, false);

    att_def = std::max(std::min(HexToNum<int>(calc_data), 9999), 0);
    att_mag = std::max(std::min(HexToNum<int>(calc_data + 0x04), 9999), 0);
    att_cou = std::max(std::min(HexToNum<int>(calc_data + 0x08), 9999), 0);

    mag_def = std::max(std::min(HexToNum<int>(calc_data + 0x0C), 9999), 0);
    mag_mag = std::max(std::min(HexToNum<int>(calc_data + 0x10), 9999), 0);
    mag_cou = std::max(std::min(HexToNum<int>(calc_data + 0x14), 9999), 0);

    str_def = std::max(std::min(HexToNum<int>(calc_data + 0x18), 9999), 0);
    str_mag = std::max(std::min(HexToNum<int>(calc_data + 0x1C), 9999), 0);
    str_cou = std::max(std::min(HexToNum<int>(calc_data + 0x20), 9999), 0);
}

void DamageCalculations::DisplayCalcs(const Fighter& Attacker, const Fighter& Defender)
{
    std::cout << std::endl <<
        Attacker.name << " VS " << Defender.name << std::endl <<
        "Attack - Defend: " << att_def << std::endl <<
        "Attack - Magic: " << att_mag << std::endl <<
        "Attack - Counter: " << att_cou << std::endl;

    std:: cout <<
        "Magic - Defend: " << mag_def << std::endl <<
        "Magic - Magic: " << mag_mag << std::endl <<
        "Magic - Counter: " << mag_cou << std::endl;

    std::cout <<
        "Strike - Defend: " << str_def << std::endl <<
        "Strike - Magic: " << str_mag << std::endl <<
        "Strike - Counter: " << str_cou << std::endl;
}