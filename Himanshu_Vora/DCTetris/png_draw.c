#include <stdio.h>
#include <kos.h>
#include <png/png.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "png_draw.h"


int png_to_gl_texture(struct texture * tex, char const * const filename) {
	int ret = 0;
	FILE * file = 0;
	uint8_t * data = 0;
	png_structp parser = 0;
	png_infop info = 0;
	png_bytep * row_pointers = 0;

	png_uint_32 w, h;
	int bit_depth;
	int color_type;


	if(!tex || !filename) {
		CLEANUP(1);
	}

	file = fopen(filename, "rb");
	if(!file) {
		CLEANUP(2);
	}

	parser = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if(!parser) {
		CLEANUP(3);
	}

	info = png_create_info_struct(parser);
	if(!info) {
		CLEANUP(4);
	}

	if(setjmp(png_jmpbuf(parser))) {
		CLEANUP(5);
	}

	png_init_io(parser, file);
	png_read_info(parser, info);
	png_get_IHDR(parser, info, &w, &h, &bit_depth, &color_type, 0, 0, 0);

	if((w & (w-1)) || (h & (h-1)) || w < 8 || h < 8) {
		CLEANUP(6);
	}

	if(png_get_valid(parser, info, PNG_INFO_tRNS) || (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) || color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_expand(parser);
	}
	if(bit_depth == 16) {
		png_set_strip_16(parser);
	}
	if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
		png_set_gray_to_rgb(parser);
	}
	png_read_update_info(parser, info);

	int rowbytes = png_get_rowbytes(parser, info);
	rowbytes += 3 - ((rowbytes-1) % 4); // align to 4 bytes

	data = malloc(rowbytes * h * sizeof(png_byte) + 15);
	if(!data) {
		CLEANUP(7);
	}

	row_pointers = malloc(h * sizeof(png_bytep));
	if(!row_pointers) {
		CLEANUP(8);
  }

	// set the individual row_pointers to point at the correct offsets of data
	for(png_uint_32 i = 0; i < h; ++i) {
		row_pointers[h - 1 - i] = data + i * rowbytes;
	}

	png_read_image(parser, row_pointers);

	// Generate the OpenGL texture object
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	GLenum texture_format = (color_type & PNG_COLOR_MASK_ALPHA) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, texture_format, w, h, 0, texture_format, GL_UNSIGNED_BYTE, data);

	tex->id = texture_id;
	tex->w = w;
	tex->h = h;
	tex->format = texture_format;
	tex->min_filter = tex->mag_filter = GL_NEAREST;


cleanup:
	if(parser) {
		png_destroy_read_struct(&parser, info ? &info : 0, 0);
	}

	if(row_pointers) {
		free(row_pointers);
	}

	if(ret && data) {
		free(data);
	}

	if(file) {
		fclose(file);
	}

	return ret;
}

void draw_textured_quad(struct texture const * const tex, float x0, float y0) {
	float x1 = x0 + tex->w;
	float y1 = y0 + tex->h;

	float const vertex_data[] = {
		/* 2D Coordinate, texture coordinate */
		x0, y1,  0.f, 1.f,
		x0, y0,  0.f, 0.f,
		x1, y1,  1.f, 1.f,
		x1, y0,  1.f, 0.f,
	};

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex->min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex->mag_filter);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer  (2, GL_FLOAT, 4 * sizeof(float), vertex_data);
	glTexCoordPointer(2, GL_FLOAT, 4 * sizeof(float), vertex_data + 2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

