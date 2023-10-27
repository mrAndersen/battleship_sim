#include <iostream>
#include <csignal>
#include "Game.h"

void Game::render() {
    p1.render();
    p2.render();
    printf("winner=%d\n", winner);
}


Game::Game() {
}


void Game::play(float step) {
    bool playing = true;

    auto renderStep = [&]() {
        if (step > 0) {
            std::cout << "\x1B[2J\x1B[H";
            render();
            usleep(step * 1000000);
        }
    };

    while (playing) {
        p1.doMove(&p2);
        renderStep();

        if (p2.deadCells == maxDeadCells) {
            winner = 1;
            playing = false;
        }

        p2.doMove(&p1);
        renderStep();

        if (p1.deadCells == maxDeadCells) {
            winner = 2;
            playing = false;
        }

        if (p1.deadCells == maxDeadCells && p2.deadCells == maxDeadCells) {
            winner = 3;
            playing = false;
        }
    }
}