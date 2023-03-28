#pragma once

#include <kos.h>
#include <stdio.h>

#include "utils/drawing.h"

class Player {
private:
    int x, y;
    int width, height;
    Color color;

public:
    Player(int x, int y);
public:
    void move();

public:
    inline int getPlayerX() { return x; }
    inline int getPlayerY() { return y; }
};