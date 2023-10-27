#include "PlayerState.h"

void PlayerState::render() {
    field.render(std::to_string(playerIndex));
    printf("moves=%d,deadCells=%d\n", moves, deadCells);
}

void PlayerState::doMove(PlayerState *target) {
    auto move = pickFreeMove();
    moves++;

    if (!move.has_value()) {
        return;
    }

    auto movePoint = move.value();

    if (target->field.isFree(movePoint)) {
        target->field.markShotFired(movePoint);
        return;
    }

    target->deadCells++;
    target->field.markShipDamage(movePoint);
    shipBuffer.emplace_back(movePoint);

    return doMove(target);
}

void PlayerState::removeFreeMove(const Point &p) {
    if (p.isOutOfBonds()) {
        return;
    }

    std::string key = std::to_string(p.x) + "|" + std::to_string(p.y);

    if (freeMoves.contains(key)) {
        freeMoves.erase(key);
    }
}

std::optional<Point> PlayerState::pickFreeMove() {
    if (freeMoves.empty()) {
        return {};
    }

    auto random = std::next(std::begin(freeMoves), getRandom(0, freeMoves.size() - 1));

    if (random == freeMoves.end()) {
        return {};
    }

    Point p;
    p.x = std::stoi(random->first.substr(0, 1));
    p.y = std::stoi(random->first.substr(2, 1));

    removeFreeMove(p);

    return p;
}