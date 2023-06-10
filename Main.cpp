#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include <thread>

#include "Utils.h"
#include "MainClasses.h"

// const addresses of player combat data
const int ATTACKERADDRESS = 0x0057CAEC;
const int DEFENDERADDRESS = 0x0057CB2C;

// battle ui flag address
const int PREDICTIONFLAGADD = 0x0057C62B;

int main()
{
    // initialize the accessor and hook to dolphin
    DolphinComm::DolphinAccessor dokapon;
    dokapon.hook();
    if (dokapon.getStatus() == DolphinComm::DolphinAccessor::DolphinStatus::hooked)
    {
        std::cout <<
            "Dolphin has been hooked." << std::endl <<
            std::endl <<
            "Open the Predictions menu in-game to see the game's damage calculations." << std::endl <<
            "Open the Info menu in-game to see extra combatant data." << std::endl;
    }

    // set the thread delay
    std::chrono::seconds hook_delay(1);
    std::chrono::milliseconds poll_delay(100);

    // create the fighter objects
    Fighter Attacker(ATTACKERADDRESS, &dokapon);
    Fighter Defender(DEFENDERADDRESS, &dokapon);

    // create the calculation object
    DamageCalculations Calcs(&dokapon);

    // main loop; exits when the user closes the window
    // sleeps for a second each iteration so the user's cpu isn't dragged down by constant polls
    int previous_flag = 0;
    while(true)
    {
        // if dolphin is not hooked, try to hook to it
        if (dokapon.getStatus() != DolphinComm::DolphinAccessor::DolphinStatus::hooked)
        {
            dokapon.unHook();
            dokapon.hook();

            if (dokapon.getStatus() != DolphinComm::DolphinAccessor::DolphinStatus::hooked)
                std::cout << "Dolphin is not hooked." << std::endl;
            else
            {
                std::cout <<
                    "Dolphin has been hooked." << std::endl <<
                    std::endl <<
                    "Open the Predictions menu in-game to see the game's damage calculations." << std::endl <<
                    "Open the Info menu in-game to see extra combatant data." << std::endl;
            }

            std::this_thread::sleep_for(hook_delay);
            continue;
        }

        char check_add[1];
        dokapon.readFromRAM((unsigned int)PREDICTIONFLAGADD, check_add, 1, false);
        char prediction_flag = HexToNum<char>(check_add);
        if (prediction_flag > 2 && prediction_flag != previous_flag)
        {
            Attacker.ReadBattleData();
            Attacker.ReadExternalData();
            Defender.ReadBattleData();
            Defender.ReadExternalData();

            if (prediction_flag == 5)
            {
                Attacker.DisplayInfo();
                Defender.DisplayInfo();
            }
            else
            {
                Calcs.ReadCalcs();
                Calcs.DisplayCalcs(Attacker, Defender);
            }
        }
        previous_flag = prediction_flag;
        std::this_thread::sleep_for(poll_delay);
    }
}

