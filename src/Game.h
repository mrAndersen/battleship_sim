#pragma once

#include "PlayerState.h"

class Game {
public:
    int maxDeadCells = 20;
    int winner = 0;

    PlayerState p1 = PlayerState(1);
    PlayerState p2 = PlayerState(2);

    Game();

    void render();

    void play(float step = 0.1);
};