#include <stdio.h>
#include <kos.h>
#include <png/png.h>
#include <mp3/sndserver.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "png_draw.h"
#include "tetris.h"

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);
extern struct game gameState;
int main(int argc, char **argv) {
	//struct texture tex;
	//maple_init();
	glKosInit();
	glTranslatef(-1.f, -1.f, 0.f);
	glScalef(1.f/320.f, 1.f/240.f, 1.f);

	if (game_init()) {
		return 1; 
	}

	snd_stream_init();
	mp3_init();
	mp3_volume(128);
	mp3_start("/rd/tetris_A.mp3", 1);

	sfxhnd_t drop_sound = snd_sfx_load("/rd/song.wav");
	struct maple_device * cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

	cont_state_t* cond;
	volatile int timer = 1000; 
	//int loc = 0; 
	for(;;) {
		cond = (cont_state_t*)maple_dev_status(cont);
		if (!gameState.block_selected) {
			choose_block();
		}
		
		if ((cond->buttons & CONT_DPAD_LEFT) || (cond->buttons & CONT_DPAD_RIGHT)) {
			move_h((cond->buttons & CONT_DPAD_LEFT)?1:0);
		} else if ((cond->buttons & CONT_DPAD_UP) || (cond->buttons & CONT_DPAD_DOWN)) {
			rotate_block((cond->buttons & CONT_DPAD_LEFT)?1:0);
		}

		if (drop()) {
			snd_sfx_play(drop_sound, 120, 0);
		}
		clear_row();
		draw_board();
		glutSwapBuffers();
		thd_sleep(2500);
	}
	snd_sfx_unload_all();
	mp3_stop();
    mp3_shutdown();
    snd_stream_shutdown();
	//glKosShutdown();
	return 0;
}
