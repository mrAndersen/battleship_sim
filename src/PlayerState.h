#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <Field.h>
#include <optional>

class PlayerState {
public:
    std::unordered_map<std::string, bool> freeMoves = {};

    Field field;

    int playerIndex = 1;

    int moves = 0;

    int deadCells = 0;

    std::vector<Point> shipBuffer = {};

    explicit PlayerState(int playerIndex) : playerIndex(playerIndex) {
        field = Field::generate();

        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                std::string key = std::to_string(i) + "|" + std::to_string(j);
                freeMoves[key] = true;
            }
        }
    }

    void render();

    void doMove(PlayerState *target);

    void removeFreeMove(const Point &p);

    std::optional<Point> pickFreeMove();
};