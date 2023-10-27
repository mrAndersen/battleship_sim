#include <module.h>
#include <random>

std::random_device seeder;

int getRandom(int min, int max) {
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

Point getRandomPoint() {
    return {getRandom(0, 9), getRandom(0, 9)};
}