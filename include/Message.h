#pragma once
#include <cstdint>

#include "include/Networking.h"

enum class GameMsg : uint32_t
{
    Client_InitMap,
    Client_AssignID,
    Game_UserInput
};

enum class UserInput
{
    DUMMY,
    ONE,
    TWO,
    THREE,
    FOUR,
    W,
    A,
    S,
    D,
    UP,
    LEFT,
    RIGHT,
    DOWN,
    W_UP,
    A_UP,
    S_UP,
    D_UP,
    UP_UP,
    LEFT_UP,
    RIGHT_UP,
    DOWN_UP
};
