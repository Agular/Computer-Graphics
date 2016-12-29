#ifndef BUFFER_TEST_H
#define BUFFER_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <glew.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <random>

using namespace glm;

// OpenGL functions

void init(void);
void reshape(int w, int h);
void update(int value);
void display(void);

// Input callback function
void mouse(int theButton, int State, int mouseX, int mouseY);
void motion(int mouseX, int mouseY);
void special(int specKey, int mouseX, int mouseY);
void keyboard(unsigned char theKey, int mouseX, int mouseY);

// Special functions
GLuint loadShaders(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath);
void makeCheckImage(void);
void makeRandomImage(void);

#endif 