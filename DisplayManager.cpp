#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <glew.h>
#include <freeglut.h>
#include <FreeImage.h>
#include "DisplayManager.h"
#include "Renderer.h"
using namespace std;
GLuint loadShaders(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath);


//Create the display
void DisplayManager::display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glutSwapBuffers();
}
void DisplayManager::createDisplay(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitContextVersion(4, 2);  // (4,5) (3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);  //GLUT_COMPATIBILITY_PROFILE
	glutCreateWindow(argv[0]);
	glewExperimental = GL_TRUE;
	if (glewInit()) printf("Error");
	printf("\n%s", (char*)glGetString(GL_RENDERER));
	printf("\n%s", (char*)glGetString(GL_VERSION));
	printf("\n%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	GLuint program = loadShaders("Dreiecke.vs", "Dreiecke.fs", "");
	glUseProgram(program);
	
}
//Update the display
void DisplayManager::updateDisplay(){
	glutPostRedisplay();
}
// Close the display
void DisplayManager::closeDisplay(){
	glutLeaveMainLoop();
}
bool DisplayManager::shouldClose(){
	return true;
}
void display(void){
	
}

