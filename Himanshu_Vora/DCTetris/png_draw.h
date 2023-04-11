#ifndef PNG_DRAW_H
#define PNG_DRAW_H

#include <stdio.h>
#include <kos.h>
#include <png/png.h>
#include <GL/gl.h>
#include <GL/glut.h>

#define CLEANUP(x) { ret = (x); goto cleanup; }

struct texture {
	GLuint id;
	GLenum format;
	GLenum min_filter;
	GLenum mag_filter;
	uint16_t w, h;
};

int png_to_gl_texture(struct texture * tex, char const * const filename);

void draw_textured_quad(struct texture const * const tex, float x0, float y0);

#endif