#include <iostream>
#include <random>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

std::random_device seeder;

typedef struct Point {
    int x = 0;
    int y = 0;

    bool isOutOfBonds() {
        return x < 0 || x > 9 || y < 0 || y > 9;
    }
} Point;

int getRandom(int min, int max) {
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

Point getRandomPoint() {
    return {getRandom(0, 9), getRandom(0, 9)};
}

typedef struct Field {
    int arr[10][10] = {};

    u64 randoms = 0;
    int deadCells = 0;

    bool isFree(const Point &point) {
        return arr[point.x][point.y] == 0;
    }

    bool isShipDead(const Point &point) {
        return !isFree(point) && isFreeArea(point);
    }

    std::vector<Point> getArea(const Point &point) {
        return {
                {point.x + 1, point.y},
                {point.x - 1, point.y},
                {point.x,     point.y + 1},
                {point.x,     point.y - 1},
                {point.x + 1, point.y - 1},
                {point.x + 1, point.y + 1},
                {point.x - 1, point.y + 1},
                {point.x - 1, point.y + 1},
        };
    }

    bool isFreeArea(const Point &point) {
        for (auto &p: getArea(point)) {
            if (!p.isOutOfBonds() && arr[p.x][p.y] == 1) {
                return false;
            }
        }

        return true;
    }

    Point getRandomStart() {
        auto r = getRandomPoint();

        while (!isFreeArea(r)) {
            r = getRandomPoint();
        }

        return r;
    }

    Point getRandomNonFired() {
        auto r = getRandomPoint();

        while (arr[r.x][r.y] == 2) {
            r = getRandomPoint();
        }

        return r;
    }

    Point getRandomEmpty() {
        auto r = getRandomPoint();

        while (!isFree(r)) {
            r = getRandomPoint();
        }

        return r;
    }

    void render(const std::string &alias = "") {
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
                    row += R"(❌)";
                }

                //shot fired at ship
                if (arr[i][j] == 3) {
                    row += R"(⭕️)";
                }
            }

            printf("%s\n", row.c_str());
        }
    }

    u64 getHash() const {
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

    void markShipArea(const std::vector<Point> &points) {
        for (auto &p: points) {
            auto local = getArea(p);

            for (auto &l: local) {
                arr[l.x][l.y] = 2;
            }

            arr[p.x][p.y] = 3;
        }
    }

    void markShipDamage(const Point &p) {
        arr[p.x][p.y] = 3;
    }

    void markShotFired(const Point &p) {
        arr[p.x][p.y] = 2;
    }

    void place(int shipSize = 4) {
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

    static Field generate() {
        Field field;

        field.place(4);

        field.place(3);
        field.place(3);

        field.place(2);
        field.place(2);
        field.place(2);

        field.place(1);
        field.place(1);
        field.place(1);
        field.place(1);

        return field;
    }
} Field;


typedef struct Game {
    int maxDeadCells = 4 + 3 + 3 + 2 + 2 + 2 + 1 + 1 + 1 + 1;
    int winner = 0;

    int p1Moves = 0;
    int p2Moves = 0;

    Field p1 = Field::generate();
    Field p2 = Field::generate();

    void render() {
        p1.render("p1");
        p2.render("p2");
        printf("w=%d,p1Moves=%d,p2Moves=%d\n", winner, p1Moves, p2Moves);
    }

    void play() {
        while (winner == 0) {
            auto p1Move = p2.getRandomNonFired();
            std::vector<Point> p1Buffer = {p1Move};
            p1Moves++;
            p2.markShotFired(p1Move);

            while (!p2.isFree(p1Move)) {
                p2.deadCells++;
                p2.markShipDamage(p1Move);

                if (p2.isShipDead(p1Move)) {
                    p2.markShipArea(p1Buffer);
                    p1Buffer.clear();
                }

                if (p2.deadCells == maxDeadCells) {
                    winner = 1;
                    break;
                }

                p1Move = p2.getRandomNonFired();
                p1Buffer.emplace_back(p1Move);
                p1Moves++;
                p2.markShotFired(p1Move);
            }

            auto p2Move = p1.getRandomNonFired();
            std::vector<Point> p2Buffer = {p2Move};
            p2Moves++;
            p1.markShotFired(p1Move);

            while (!p1.isFreeArea(p2Move)) {
                p1.deadCells++;
                p1.markShipDamage(p2Move);

                if (p1.isShipDead(p1Move)) {
                    p1.markShipArea(p2Buffer);
                    p2Buffer.clear();
                }

                if (p1.deadCells == maxDeadCells) {
                    winner = 2;
                    break;
                }

                p2Move = p1.getRandomNonFired();
                p2Buffer.emplace_back(p2Move);
                p2Moves++;
                p1.markShotFired(p1Move);
            }
        }
    }
} Game;


int main() {
    Game g;

    g.play();
    g.render();

    return 0;
}
