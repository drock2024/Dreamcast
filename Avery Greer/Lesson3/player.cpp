#include "player.h"

#include "utils/drawing.h"
#include "utils/input.h"

Player::Player(int xPos, int yPos) {
    x = xPos;
    y = yPos;
    width = 50;
    height = 50;
    Color color;
    color.red = 20;
    color.green = 20;
    color.blue = 20;
    drawBlk(x, y, width, height, color);
}

void Player::move()
{
    checkForInpUpd(x, y, width, height, color, Color {0, 0, 0});
}