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

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString()
{
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::string(); //No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    std::string message(messageBuffer, size);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}



// Low-level structs and classes:
struct Key
{
    bool pressed, held, released;
};

struct Input
{
    Key w, a, s, d,
        leftMouse, middleMouse, rightMouse;
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

        ReadKey(leftMouse, VK_LBUTTON);
        ReadKey(middleMouse, VK_MBUTTON);
        ReadKey(rightMouse, VK_RBUTTON);
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

#pragma region operators

    Vec2 operator + (const Vec2& other)
    {
        return { x + other.x, y + other.y };
    }

    Vec2 operator - (const Vec2& other)
    {
        return { x - other.x, y - other.y };
    }

    Vec2 operator * (const Vec2& other)
    {
        return { x * other.x, y * other.y };
    }

    Vec2 operator / (const Vec2& other)
    {
        return { x / other.x, y / other.y };
    }

    Vec2 operator / (const int& scaler)
    {
        return { x / scaler, y / scaler };
    }

    bool operator == (const Vec2& other)
    {
        return x == other.x && y == other.y;
    }

    bool operator != (const Vec2& other)
    {
        return x != other.x || y != other.y;
    }

    void operator += (const Vec2& other)
    {
        *this = *this + other;
    }

    void operator -= (const Vec2& other)
    {
        *this = *this - other;
    }

    void operator *= (const Vec2& other)
    {
        *this = *this * other;
    }

    void operator /= (const Vec2& other)
    {
        *this = *this / other;
    }

#pragma endregion

#pragma region Math

    int Squagnitude()
    {
        return std::max(labs(x), labs(y));
    }

    Vec2 Squarmalized()
    {
        return *this / Squagnitude();
    }

    Vec2 DirTo(Vec2 endPoint)
    {
        return (endPoint - *this).Squarmalized();
    }

    int Squistance(Vec2 other)
    {
        return (*this - other).Squagnitude();
    }

#pragma endregion
};

#pragma region Early Variables

bool shouldRun = true;
float tTime = 0.0f;

#pragma endregion