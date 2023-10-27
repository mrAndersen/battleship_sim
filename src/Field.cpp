#include "Field.h"

bool Field::isFree(const Point &point) {
    return arr[point.x][point.y] == 0;
}

bool Field::isShipDead(const Point &point) {
    auto freeCells = getFreeAroundCells(point);
    return !isFree(point) && (freeCells == 7 || freeCells == 8);
}

int Field::getFreeAroundCells(const Point &point) {
    int result = 0;

    for (auto &p: point.getArea()) {
        if (p.isOutOfBonds()) {
            result++;
            continue;
        }

        if (arr[p.x][p.y] != 1) {
            result++;
        }
    }

    return result;
}

bool Field::isFreeArea(const Point &point) {
    for (auto &p: point.getArea()) {
        if (!p.isOutOfBonds() && arr[p.x][p.y] == 1) {
            return false;
        }
    }

    return true;
}

Point Field::getRandomStart() {
    auto r = getRandomPoint();

    while (!isFreeArea(r)) {
        r = getRandomPoint();
    }

    return r;
}

void Field::render(const std::string &alias) {
    printf("p=%s,h=%zu,r=%zu\n", alias.c_str(), getHash(), randoms);

    for (int i = 0; i < 10; i++) {
        std::string row = {};

        for (int j = 0; j < 10; j++) {
            //empty
            if (arr[i][j] == 0) {
                row += R"(🆗️)";
            }

            //alive ship
            if (arr[i][j] == 1) {
                row += R"(✅)";
            }

            //shot fired
            if (arr[i][j] == 2) {
                row += R"(⭕️)";
            }

            //shot fired at ship
            if (arr[i][j] == 3) {
                row += R"(❌)";
            }
        }

        printf("%s\n", row.c_str());
    }
}

u64 Field::getHash() const {
    u64 result = 0;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int denominator = 1;

            if (arr[i][j] == 1) {
                denominator = 2;
            }

            result += i * j * denominator;
        }
    }

    return result;
}

void Field::markShipArea(const std::vector<Point> &points) {
    for (auto &p: points) {
        auto local = p.getArea();

        for (auto &l: local) {
            arr[l.x][l.y] = 2;
        }

        arr[p.x][p.y] = 3;
    }
}

void Field::markShipDamage(const Point &p) {
    arr[p.x][p.y] = 3;
}

void Field::markShotFired(const Point &p) {
    arr[p.x][p.y] = 2;
}

void Field::place(int shipSize) {
    Point start = {};

    bool placing = true;
    int orientation = 0;

    do {
        start = getRandomStart();
        orientation = getRandom(0, 1);
        randoms++;

        if (!isFree(start)) {
            continue;
        }

        if (orientation == 0) {
            if (start.y + shipSize - 1 > 9) {
                continue;
            }

            int freeSlots = 0;

            for (int i = 0; i < shipSize; ++i) {
                if (isFreeArea({start.x, start.y + i})) {
                    freeSlots++;
                }
            }

            if (freeSlots == shipSize) {
                for (int i = 0; i < shipSize; ++i) {
                    arr[start.x][start.y + i] = 1;
                }

                placing = false;
            } else {
                continue;
            }
        }

        if (orientation == 1) {
            if (start.x + shipSize - 1 > 9) {
                continue;
            }

            int freeSlots = 0;

            for (int i = 0; i < shipSize; ++i) {
                if (isFreeArea({start.x + i, start.y})) {
                    freeSlots++;
                }
            }

            if (freeSlots == shipSize) {
                for (int i = 0; i < shipSize; ++i) {
                    arr[start.x + i][start.y] = 1;
                }

                placing = false;
            } else {
                continue;
            }
        }

    } while (placing);

    randoms += randoms;
}