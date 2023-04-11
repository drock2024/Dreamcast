#define PACK_PIXEL(r, g, b) ( ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)) //convert 16 bit color form of DC to 3 bits

#include <kos.h> //Include KallistiOS
#include <stdio.h>

int main(void) {
    int quit = 0;
    //vars for tracking pixel
    int i = 0; int j = 0;
    bool check = false;
    bool startCheck = false;

    //init kos
    pvr_init_defaults();

    //set our video mode
    vid_set_mode(DM_320x240, PM_RGB565);

    //Keep drawing frames
    while(!quit) {
        for(i = 0; i < 320; i++) {
            //Even columns red, odd columns blue
            if ((i/10) % 2 == 0) {
                check = false;   
            } else {
                check = true;
            }
            startCheck = check;
            for(j = 0; j < 240; j++) {
                //On odd rows, flip
                if((j/10) % 2 == 1) {
                    check = !startCheck;
                } else {
                    check = startCheck;
                }
                if (check) {
                    vram_s[i + (j * 320)] = PACK_PIXEL(0, 0, 255);
                } else {
                    vram_s[i + (j * 320)] = PACK_PIXEL(255, 0, 0);
                }
            }
        }
    }
    return 0;
}

