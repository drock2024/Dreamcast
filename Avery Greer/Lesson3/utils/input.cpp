#include <kos.h>

#include "input.h"
#include "drawing.h"
#include "globals.h"

void checkForInpUpd(int& blk_xPos, int& blk_yPos, int blkWidth, int blkHeight, Color color, Color fadeIncAmnt)	//Check for input from user and update block position if needed
{
	//Get input from any controller
    MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER,	//Type of device
                        cont_state_t,	//Device status cast to type (controller status)
                        st	//Device status
                        )	//Get input from any controller

    if(st->buttons & CONT_DPAD_UP) {	//D-pad up pressed
        if (blk_yPos > 0) {
            moveBlk(
                blk_xPos,
                blk_yPos,
                blk_xPos,
                blk_yPos-1,
                blkWidth,
                blkHeight,
                color,
                fadeIncAmnt
            );	//Remove block from old position and draw in new position
            blk_yPos--;
        }
    } else {
        if(st->buttons & CONT_DPAD_DOWN) {	//D-pad down pressed
            if (blk_yPos + blkHeight < scrnHeight) {
                moveBlk(
                    blk_xPos,
                    blk_yPos,
                    blk_xPos,
                    blk_yPos+1,
                    blkWidth,
                    blkHeight,
                    color,
                    fadeIncAmnt
                );	//Remove block from old position and draw in new position
                blk_yPos++;
            }
        } else {
            if(st->buttons & CONT_DPAD_LEFT) {	//D-pad left pressed
                if (blk_xPos > 0) {
                    moveBlk(
                        blk_xPos,
                        blk_yPos,
                        blk_xPos-1,
                        blk_yPos,
                        blkWidth,
                        blkHeight,
                        color,
                        fadeIncAmnt
                    );	//Remove block from old position and draw in new position
                    blk_xPos--;
                }
            } else {
                if(st->buttons & CONT_DPAD_RIGHT) {	//D-pad right pressed
                    if (blk_xPos + blkWidth < scrnWidth) {
                        moveBlk(
                            blk_xPos,
                            blk_yPos,
                            blk_xPos+1,
                            blk_yPos,
                            blkWidth,
                            blkHeight,
                            color,
                            fadeIncAmnt
                        );	//Remove block from old position and draw in new position
                        blk_xPos++;
                    }
                }
            }
        }
    }
        

    MAPLE_FOREACH_END()
}