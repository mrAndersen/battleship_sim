#include <thread>
#include "Game.h"

void simulate(int games = 1000) {
    int w1 = 0;
    int w2 = 0;
    int w3 = 0;

    for (int i = 0; i < games; ++i) {
        Game g;
        g.play();

        if (g.winner == 1) {
            w1++;
        }

        if (g.winner == 2) {
            w2++;
        }

        if (g.winner == 3) {
            w3++;
        }

        printf("\r%d\\%d", i + 1, games);
    }

    printf("\nW1=%d, W2=%d, W3=%d\n", w1, w2, w3);

}

void singleGame() {
    Game g;

    g.play(.5);
}

void simulateThreaded() {
    int threads = 5;
    std::atomic<int> finished = 0;

    for (int i = 0; i < threads; ++i) {
        std::thread([&]() {
            simulate();
            finished++;
        }).detach();
    }

    while (finished != threads) {
        usleep(10000);
    }
}


int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    singleGame();


    return 0;
}
