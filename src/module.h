#pragma once

#include <vector>
#include <string>
#include <cstdint>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

struct Point {
    int x = 0;
    int y = 0;

    bool isOutOfBonds() const {
        return x < 0 || x > 9 || y < 0 || y > 9;
    }

    std::vector<Point> getArea() const {
        return {
                {x + 1, y},
                {x - 1, y},
                {x,     y + 1},
                {x,     y - 1},

                {x + 1, y - 1},
                {x + 1, y + 1},
                {x - 1, y + 1},
                {x - 1, y - 1},
        };
    }
};

int getRandom(int min, int max);

Point getRandomPoint();