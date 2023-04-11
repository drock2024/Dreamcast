/*******************************************************************************
	Sega Dreamcast Project

	Project name : DC Game
	Created on   : 2022-11-06
*******************************************************************************/

#define PROJECT_NAME "DC Game"

#include <kos.h>

// kosext2fs
#include <ext2/fs_ext2.h>

// kosutils
#include <kos/bspline.h>
#include <kos/img.h>
#include <kos/md5.h>
#include <kos/netcfg.h>
#include <kos/pcx.h>
#include <kos/vector.h>

// time
#include <time.h>

// math
#include <math.h>

/* Your program's main entry point */

const int scrnWidth = 320;
const int scrnHeight = 240;
const int blkWidth = 32;
const int blkHeight = 32;
const int waitTime = 2000;

void drawBlk(int x, int y, int width, int height, int r, int g, int b, int rIncAmt, int gIncAmt, int bIncAmt) {
    for (int yPos = 0; yPos < height; yPos++) {
        for (int xPos = 0; xPos < width; xPos++) {
            vram_s[(x + xPos) + ((y * scrnWidth) + (yPos * scrnWidth))] = ((r & 0x1F) << 10) | ((g & 0x1F) << 5) | (b & 0x1F);
            r += rIncAmt;
            g += gIncAmt;
            b += bIncAmt;
        }

        r = g = b = 0;
    }
}

void moveBlk(int xOld, int yOld, int xNew, int yNew, int width, int height, int r, int g, int b, int rIncAmt, int gIncAmt, int bIncAmt) {
    drawBlk(xOld, yOld, blkWidth, blkHeight, 0, 0, 0, 0, 0, 0);
    drawBlk(xNew, yNew, blkWidth, blkWidth, r, g, b, rIncAmt, gIncAmt, bIncAmt);
}

void checkForInpUpd(int& blk_xPos, int& blk_yPos) {
    MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)

        if (st->buttons & CONT_DPAD_UP) {
            if (blk_yPos > 0) {
                moveBlk(blk_xPos, blk_yPos, blk_xPos, blk_yPos - 1, blkWidth, blkHeight, 0, 0, 0, 1, 0, 0);
                blk_yPos--;
            }
        } else {
            if (st->buttons & CONT_DPAD_DOWN) {
                if (blk_yPos + blkHeight < scrnHeight) {
                    moveBlk(blk_xPos, blk_yPos, blk_xPos, blk_yPos + 1, blkWidth, blkHeight, 0, 0, 0, 1, 0, 0);
                    blk_yPos++;
                }
            } else {
                if (st->buttons & CONT_DPAD_LEFT) {
                    if (blk_xPos > 0) {
                        moveBlk(blk_xPos, blk_yPos, blk_xPos - 1, blk_yPos, blkWidth, blkHeight, 0, 0, 0, 1, 0, 0);
                        blk_xPos--;
                    }
                } else {
                    if (st->buttons & CONT_DPAD_RIGHT) {
                        if (blk_xPos + blkWidth < scrnWidth) {
                            moveBlk(blk_xPos, blk_yPos, blk_xPos + 1, blk_yPos, blkWidth, blkHeight, 0, 0, 0, 1, 0, 0);
                            blk_xPos++;
                        }
                    }
                }
            }
        }

    MAPLE_FOREACH_END()
}
int main(void) {

    /* Your program start here... */
    printf("\nHello world from %s!\n\n", PROJECT_NAME);

    int quit = 0;
    int blk_xPos = 0;
    int blk_yPos = 0;

    pvr_init_defaults();

    vid_set_mode(DM_320x240, PM_RGB555);

    drawBlk(blk_xPos, blk_yPos, blkWidth, blkHeight, 0, 0, 0, 1, 0, 0);

    while(!quit) {
        usleep(waitTime);
        checkForInpUpd(blk_xPos, blk_yPos);
    }
	/* Bye... */
    return 0;
}
