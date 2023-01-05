#include <chrono> // Nice time lib
#include <thread> // Helper library for chrono me thinks.
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds
/*
sleep_for(nanoseconds(10));
sleep_until(system_clock::now() + seconds(1));
*/
#include <stdio.h>      // printf, scanf, puts, NULL
#include <stdlib.h>     // srand, rand
#include <time.h>       // time(NULL)
#include <string> // for string class
#include <vector> // For vector class.
#include <iostream> // For cin and cout.
#include <algorithm>
#include <stdexcept>
#include "cpp-terminal/base.hpp" // Fancy colors. =]
#include "cpp-terminal/window.hpp"
#include "cpp-terminal/input.hpp"
#define T_RESET "\033[0m"
#define NOMINMAX
#include <Windows.h>

enum Button
{
    ButtonConfirm
};

using std::string;
using std::vector;
using std::to_string;
using std::cout;
using std::cin;
using std::isdigit;
using std::stoi;

typedef unsigned int uint;
typedef uint8_t byte;
// Early functions:
// Early functions.
int JMod(int a, int b)
{
    return (a % b + b) % b;
}

int RandRange(int min, int max)
{
    return (min == max ? min : (rand() % (max - min + 1)) + min);
}

bool IsNumber(const string& s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}



// Low-level structs and classes:
struct Key
{
    bool pressed, held, released;
};

struct Input
{
    Key w, a, s, d;
    Input() = default;

    static bool FindKey(char key)
    {
        return GetKeyState(key) & 0x8000;
    }

    void ReadKey(Key& key, char keyCharacter)
    {
        bool newPressed = Input::FindKey(keyCharacter);
        key.pressed = !key.held && newPressed;
        key.released = key.held && !newPressed;
        key.held = newPressed;
    }

    void Update()
    {
        ReadKey(w, 'W');
        ReadKey(a, 'A');
        ReadKey(s, 'S');
        ReadKey(d, 'D');
    }
};

class Settings
{
public:
    int sleepTime;

    Settings(int sleepTime) :
        sleepTime(sleepTime)
    { }

    Settings() = default;
};

class Vec2
{
public:
    int x, y;

    Vec2(int x = 0, int y = 0) : x(x), y(y) { }
};

#pragma region Early Variables

bool shouldRun = true;

#pragma endregion