//
//  main.cpp
//  ogamalBreakout
//
//  Created by Osama Attia on 9/21/14.
//  ogamal@iastate.edu
//

#include <iostream>
#include <assert.h>
#include "math.h"
#if defined(__APPLE__) || defined(MACOSX)
	#include <GLUT/glut.h>
#elif defined(_arch_dreamcast)

	#include <kos.h>

    #ifndef BUILD_LIBGL
    #include <GL/glkos.h>
    #include <GL/glext.h>
    #else
    #include "glkos.h"
    #include "glext.h"
    #endif

    #ifndef BUILD_LIBGL
    #include <GL/gl.h>
    #include <GL/glu.h>
    #else 
    #include "gl.h"
    #include "glu.h"
    #endif
    #else
	#include "glut.h"
#endif

#include "Breakout.h"


Breakout game;

// Define the display function
void myDisplay()
{
	game.display();
}

// Define the reshape function
void myReshape(int width, int height)
{
	game.reshape(width, height);
}

// Define the mouse click events
void myMouseClick(int button, int state, int x, int y)
{
	game.mouseClick(button, state, x, y);
}

// Define the mouse drag events
void myMouseMove(int x, int y)
{
	game.mouseMove(x, y);
}

// Define keystroke events
void myKeyStroke(unsigned char key, int x, int y)
{
	game.keyStroke(key, x, y);
}

void mySpecialKeyStroke(int key, int x, int y)
{
	game.specialKeyPos(key, x, y);
}

void dc_myKeyStroke()
{
	int buttons;
	maple_device_t *cont;
	cont_state_t *state;

	cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
	if(!cont) return;
	state = (cont_state_t *)maple_dev_status(cont);

	buttons = state->buttons;
	//Add triggers as possible input
	if (state->ltrig>0) buttons|=(1<<16);
	if (state->rtrig>0) buttons|=(1<<17);

	if(buttons & CONT_START){
		game.keyStroke('n', 0, 0);
	}

	if(buttons & CONT_Y){
		game.keyStroke('q',0,0);
	}

	if(buttons & CONT_X){
		game.keyStroke('h',0,0);
	}

	if(buttons & (1<<17)){
		game.newBall(rand()%640, rand()%480);
	}
}

void dc_mySpecialKeyStroke()
{
	maple_device_t *cont;
	cont_state_t *state;

	cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
	if(!cont) return;
	state = (cont_state_t *)maple_dev_status(cont);

	if( (state->joyx < -16) || (state->buttons & CONT_DPAD_LEFT) ){
		game.specialKeyPos(GLUT_KEY_LEFT, 0, 0);
	}else if( (state->joyx > 16) || (state->buttons & CONT_DPAD_RIGHT) ){
		game.specialKeyPos(GLUT_KEY_RIGHT, 0, 0);
	}
}

// The main function here
int main(int argc, char ** argv)
{
	#ifdef _arch_dreamcast

	glKosInit(); // Setup GLdc

	game.init();

	while (1)
	{
		myDisplay();
		dc_myKeyStroke();
		dc_mySpecialKeyStroke();
		glKosSwapBuffers();
	}

	#else
	// Init glut
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	// Init display mode

	// Init window size, position, title
	glutInitWindowSize(WINWIDTH, WINHEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(WINTITLE);
    
	// Init game
	game.init();

	// Draw scene
	glutDisplayFunc(myDisplay);
	// Handle reshape
	glutReshapeFunc(myReshape);
	// Handle mouse clicks
	glutMouseFunc(myMouseClick);
	// Handle mouse motion
//	glutMotionFunc(myMouseMove);
    glutPassiveMotionFunc(myMouseMove);
	// Handle keyboard strokes
	glutKeyboardFunc(myKeyStroke);
	// specify keyboard special key input events
	glutSpecialFunc(mySpecialKeyStroke);
	// Enter the opengl event processing loop
	glutMainLoop();
	#endif

	return 0;
}
