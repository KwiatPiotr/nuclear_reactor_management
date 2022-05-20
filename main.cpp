#include <array>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include "cryptopp/osrng.h"


constexpr uint8_t MAX_T1_VALUE = 70u;
constexpr uint8_t MAX_T2_VALUE = 50u;
constexpr uint8_t MAX_T3_VALUE = 30u;
constexpr uint8_t COOLING_EFFICIENCY = 5u;

static std::array<uint8_t, 3> environment;
static std::array<bool, 3> fanState;
static bool reactorWorking = true;

void printOptions()
{
    std::cout << "What to do:\n"
                 "1. Check temperature\n"
                 "2. Quit\n";
}

void printTemperatures()
{
    std::cout << "===========================================\n";
    std::cout << std::left << std::setw(6) << "T1:";
    std::cout << std::left << std::setw(6) << "T2:";
    std::cout << std::left << std::setw(6) << "T3:";
    std::cout << std::left << std::setw(9) << "Fan 1:";
    std::cout << std::left << std::setw(9) << "Fan 2:";
    std::cout << std::left << std::setw(9) << "Fan 3:";
    std::cout << '\n';
    std::cout << std::left << std::setw(6) << static_cast<int>(environment[0]);
    std::cout << std::left << std::setw(6) << static_cast<int>(environment[1]);
    std::cout << std::left << std::setw(6) << static_cast<int>(environment[2]);
    std::cout << std::left << std::setw(9) << ((fanState[0]) ? "ON" : "OFF");
    std::cout << std::left << std::setw(9) << ((fanState[1]) ? "ON" : "OFF");
    std::cout << std::left << std::setw(9) << ((fanState[2]) ? "ON" : "OFF");
    std::cout << '\n';
}

void testTemperatures()
{
    fanState[0] = (environment[0] > MAX_T1_VALUE);

    fanState[1] = (environment[1] > MAX_T2_VALUE);

    fanState[2] = (environment[2] > MAX_T3_VALUE);
}

void doCooling()
{
    if (std::any_of(fanState.begin(), fanState.end(), [](const bool& x){ return x; }))
    {
        std::cout << "COOLING\n";
        for (int i = 0; i < fanState.size(); ++i) {
            if (fanState[i])
                environment[i] -= COOLING_EFFICIENCY;
        }
        reactorWorking = false;
    }
    else
    {
        reactorWorking = true;
    }
}

uint8_t updateTemperatures()
{
    uint8_t result;
    if (reactorWorking)
    {
        static CryptoPP::BlockingRng randGen;
        randGen.GenerateBlock(environment.data(), environment.size());

        std::for_each(environment.begin(), environment.end(),
                      [](uint8_t &x) { x = static_cast<uint8_t>(static_cast<float>(x) / 2.55f); });
    }

    if (std::all_of(environment.begin(), environment.end(), [](uint8_t &x){ return x > 1u; }))
    {
        result = 0;
    }
    else
    {
        result = 1;
    }

    testTemperatures();
    printTemperatures();

    return result;
}

uint8_t userInterface()
{
    uint8_t optionChosen;
    std::string userInput;

    printOptions();

    std::cin >> userInput;

    if (userInput == "1" || userInput == "1.")
    {
        optionChosen = 1u;
    }
    else if (userInput == "2" || userInput == "2."){
        optionChosen = 2u;
    }
    else {
        optionChosen = 0xFFu;
    }

    return optionChosen;
}


int main() {

    bool running = true;
    int errorCode = 0;

    while (running) {

        uint8_t readTemperatureStatus = updateTemperatures();
        if (readTemperatureStatus == 1)
        {
            std::cout<< "Some temperature is bellow 1\n";
        }
        doCooling();
        uint8_t action = userInterface();

        switch (action) {
            case 1:
            {
                break;
            }
            case 2:
            {
                running = false;
                break;
            }
            default:
            {
                std::cout << "Wrong option provided!!!\n";
                errorCode = 1;
                running = false;
                break;
            }
        }
    }

    while (!reactorWorking)
    {
        updateTemperatures();
        doCooling();
    }

    return errorCode;
}
