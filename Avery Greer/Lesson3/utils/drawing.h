#pragma once

typedef struct color {
    int red;
    int green;
    int blue;
} Color;



void drawBlk(int x, int y, int width, int height, Color color, Color fadeIncAmount);
void drawBlk(int x, int y, int width, int height, Color color);

void moveBlk(int xOld, int yOld, int xNew, int yNew,int width, int height, Color color, Color fadeIncAmount);