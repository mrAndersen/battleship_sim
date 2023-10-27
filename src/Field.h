#pragma once

#include <module.h>

class Field {
public:
    int arr[10][10] = {};

    u64 randoms = 0;

    bool isFree(const Point &point);

    bool isShipDead(const Point &point);

    int getFreeAroundCells(const Point &point);

    bool isFreeArea(const Point &point);

    Point getRandomStart();

    void render(const std::string &alias = "");

    u64 getHash() const;

    void markShipArea(const std::vector<Point> &points);

    void markShipDamage(const Point &p);

    void markShotFired(const Point &p);

    void place(int shipSize = 4);

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
};