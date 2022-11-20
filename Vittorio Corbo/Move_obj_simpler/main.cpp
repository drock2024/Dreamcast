/*  Move object simple
	Allow user to move a coloured block around the screen
	Supports use of a controller plugged into any port

	Follows on from Fill screen with colour project
   
	By James S.
*/

#include <kos.h>	//Access KallistiOS
#include <time.h>
#include <stdio.h>	//Standard I/O for console O/P

const int scrnWidth=320;	//Screen width
const int scrnHeight=240;	//Screen height
const int blkWidth=32;	//Block width
const int blkHeight=32;	//Block height
const int waitTime=2000;	//Delay before checking for user input (microseconds)

void drawBlk(int x, int y,int width, int height, int red, int green, int blue, int redIncAmnt, int greenIncAmnt, int blueIncAmnt){ //Draw a coloured block
	for(int yPos = 0; yPos<height; yPos++){
		for(int xPos = 0; xPos <width; xPos++){
			vram_s[(x + xPos) + ((y * scrnWidth) + (yPos * scrnWidth))] = ((red & 0x1F) << 10) | ((green & 0x1F) << 5) | (blue & 0x1F);
			red+=redIncAmnt;
			green+=greenIncAmnt;
			blue+=blueIncAmnt;
		}
		
		red=green=blue=0;
	}
}

void moveBlk(int xOld, int yOld, int xNew, int yNew,int width, int height, int red, int green, int blue, int redIncAmnt, int greenIncAmnt, int blueIncAmnt){ 	//Remove block from old position and draw in new position
	drawBlk(xOld,yOld,blkWidth,blkHeight,0,0,0,0,0,0); //Remove block from current position
	drawBlk(xNew,yNew,blkWidth,blkHeight,red,green,blue,redIncAmnt,greenIncAmnt,blueIncAmnt); 	//Draw green block at current position
}

void checkForInpUpd(int& blk_xPos, int& blk_yPos){	//Check for input from user and update block position if needed

	//Get input from any controller
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER,	//Type of device
							cont_state_t,	//Device status cast to type (controller status)
							st	//Device status
							)	//Get input from any controller

			if(st->buttons & CONT_DPAD_UP) {	//D-pad up pressed
				if (blk_yPos > 0) {
					moveBlk(blk_xPos,blk_yPos,blk_xPos,blk_yPos-1,blkWidth,blkHeight,0,0,0,0,1,0);	//Remove block from old position and draw in new position
					blk_yPos--;
				}
			}else {
				if(st->buttons & CONT_DPAD_DOWN) {	//D-pad down pressed
					if (blk_yPos + blkHeight < scrnHeight) {
						moveBlk(blk_xPos,blk_yPos,blk_xPos,blk_yPos+1,blkWidth,blkHeight,0,0,0,0,1,0);	//Remove block from old position and draw in new position
						blk_yPos++;
					}
				}else {
					if(st->buttons & CONT_DPAD_LEFT) {	//D-pad left pressed
						if (blk_xPos > 0) {
							moveBlk(blk_xPos,blk_yPos,blk_xPos-1,blk_yPos,blkWidth,blkHeight,0,0,0,0,1,0);	//Remove block from old position and draw in new position
							blk_xPos--;
						}
					}else {
						if(st->buttons & CONT_DPAD_RIGHT) {	//D-pad right pressed
							if (blk_xPos + blkWidth < scrnWidth) {
								moveBlk(blk_xPos,blk_yPos,blk_xPos+1,blk_yPos,blkWidth,blkHeight,0,0,0,0,1,0);	//Remove block from old position and draw in new position
								blk_xPos++;
							}
						}
					}
				}
			}
            

        MAPLE_FOREACH_END()
}

int main(void) {
	int quit=0;	//Quit status
	int blk_xPos=0;	//Block X position
	int blk_yPos=0;	//Block Y position
 
	pvr_init_defaults();	//Sets up the pvr core with a bunch of default settings
 
	vid_set_mode(DM_320x240, PM_RGB555);	//Set our video mode to 320 x 240, 60Hz and 15-bit (RRRRRGGGGGBBBBB)
 
	drawBlk(blk_xPos,blk_yPos,blkWidth,blkHeight,0,0,0,0,1,0); 	//Draw green block at current position
 
	//Main loop
	while(!quit) {
		usleep(waitTime);
		checkForInpUpd(blk_xPos,blk_yPos);	//Check for input from user and update block position if needed
	}
	
	return 0;
}

