
#include <stdio.h>
#include <time.h>

#include "drawing.h"
#include "globals.h"

void drawBlk(int x, int y,int width, int height, Color color, Color fadeIncAmount) //Draw a coloured block
{
	int red = color.red;
	int green = color.green;
	int blue = color.blue;
	for(int yPos = 0; yPos<height; yPos++)
	{
		for(int xPos = 0; xPos <width; xPos++)
		{
			vram_s[(x + xPos) + ((y * scrnWidth) + (yPos * scrnWidth))] = ((red & 0x1F) << 10) | ((green & 0x1F) << 5) | (blue & 0x1F);
			red += fadeIncAmount.red;
			green += fadeIncAmount.green;
			blue += fadeIncAmount.blue;
		}
		
		red = color.red;
        green = color.green;
        blue = color.blue;
	}
}

void drawBlk(int x, int y, int width, int height, Color color)
{
    drawBlk(x, y, width, height, color, Color {0, 0, 0});
}

void moveBlk(int xOld, int yOld, int xNew, int yNew,int width, int height, Color color, Color fadeIncAmount)	//Remove block from old position and draw in new position
{ 
	drawBlk(xOld,yOld,width,height,(Color) {0, 0, 0}, (Color) {0, 0, 0}); //Remove block from current position
	drawBlk(xNew,yNew,width,height,color,fadeIncAmount); 	//Draw green block at current position
}