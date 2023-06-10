#pragma once
#include <string>

#include "DolphinAccessor.h"

class Fighter
{
private:
    DolphinComm::DolphinAccessor* Game;

    unsigned int main_add;
    unsigned int struct_add;
    unsigned int weapon_add;
    unsigned int shield_add;
    unsigned int accessory_add;
    unsigned int class_add;
    unsigned int off_mag_add;
    unsigned int def_mag_add;
    unsigned int skill_add;
    unsigned int name_add;

    enum ProfID { NONE = 0, WARRIOR = 1, MAGICIAN = 2, THIEF = 3, CLERIC = 4, ACROBAT = 5, MONK = 6, NINJA = 7, ROBO = 8};
    std::string prof_names[9] = {"None", "Warrior-based", "Magician-based", "Thief", "Cleric", "Acrobat", "Monk", "Ninja", "Robo Knight"};

public:
    std::string name;
    std::string weapon_name;
    std::string shield_name;
    std::string accessory_name;
    std::string class_name;
    std::string off_mag_name;
    std::string def_mag_name;
    std::string skill_name;

    int max_hp = 0;
    int current_hp = 0;
    int off_mag_id = 0;
    int off_mag_val = 0;
    int def_mag_id = 0;
    int def_mag_val = 0;
    int skill_id = 0;

    ProfID prof_id = NONE;
    bool proficiency = false;
    bool player = false;

    Fighter(int, DolphinComm::DolphinAccessor*);

    void ReadBattleData();
    void ReadExternalData();
    void DisplayInfo();
};

class DamageCalculations
{
private:
    static const unsigned int CALC_OFFSET = 0x0057CB68;

    DolphinComm::DolphinAccessor* Game;
    Fighter* Attacker;
    Fighter* Defender;

    int att_def;
    int att_mag;
    int att_cou;
    int mag_def;
    int mag_mag;
    int mag_cou;
    int str_def;
    int str_mag;
    int str_cou;
    
public:
    DamageCalculations(DolphinComm::DolphinAccessor*);

    void ReadCalcs();
    void DisplayCalcs(const Fighter&, const Fighter&);
};