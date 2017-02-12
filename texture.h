#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

GLuint loadBMP(const char * imagepath);

GLuint loadDDS(const char * imagepath);

#endif