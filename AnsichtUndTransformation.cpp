/*
Pixel und Puffer.
Depth, Blending, Stencil testing.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glew.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <iostream>
GLuint loadShaders(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath);
GLint height = 768, width = 1366;
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition, vColor };
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint program;
const GLuint NumVertices = 72;
const GLuint nConeVertices = 1000;
GLfloat pi = 3.14159;
# define PI           3.14159265358979323846  /* pi */
GLfloat alpha = 0.2, beta = 0.8, dist = 5, DELTA = 0.5;
GLuint VAO, VBO, boxVAO, boxVBO;
GLfloat ConeVertices[nConeVertices][3];
//Parameters for cone
float radius = 0.5;
int nSpeichen = 40;
// Coordinates for the center of the bottom surface of the cone.
float xCone = -0.5;
float yCone = 0.5;
float zCone = 0.3;
// X, Y, Z for the tip of the cone.
float xTip = xCone;
float yTip = yCone;
float zTip = 0.8;
//Parameters for square
float length = 0.5;
// X, Y, Z for the left-down front facing side.
float xSquare = -0.5;
float ySquare = 0.25;
float zSquare = 0.0;
//Model
glm::mat4 model;
glm::mat4 view;
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};
void init(void)
{
	printf("\n%s", (char*)glGetString(GL_RENDERER));
	printf("\n%s", (char*)glGetString(GL_VERSION));
	printf("\n%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	program = loadShaders("wurfelUndKegelVertexShader.vs", "wurfelUndKegelFragShader.fs", "");
	// My cube vertices. They start from front side.
	// {1.0f,1.0f,1.0f},
	GLfloat vertices[NumVertices][3] = {
		{ xSquare + length, ySquare, zSquare }, { 1.0f, 0.0f, 0.0f }, { xSquare, ySquare + length, zSquare }, { 0.0f, 1.0f, 0.0f }, { xSquare, ySquare, zSquare }, { 0.0f, 0.0f, 1.0f },
		{ xSquare + length, ySquare, zSquare }, { 1.0f, 0.0f, 0.0f }, { xSquare + length, ySquare + length, zSquare }, { 0.0f, 1.0f, 0.0f }, { xSquare, ySquare + length, zSquare }, { 0.0f, 0.0f, 1.0f },

		{ xSquare + length, ySquare, zSquare - length }, { 1.0f, 0.0f, 0.0f }, { xSquare, ySquare + length, zSquare - length }, { 0.0f, 1.0f, 0.0f }, { xSquare, ySquare, zSquare - length }, { 0.0f, 0.0f, 1.0f },
		{ xSquare + length, ySquare, zSquare - length }, { 1.0f, 0.0f, 0.0f }, { xSquare + length, ySquare + length, zSquare - length }, { 0.0f, 1.0f, 0.0f }, { xSquare, ySquare + length, zSquare - length }, { 0.0f, 0.0f, 1.0f },

		{ xSquare + length, ySquare + length, zSquare }, { 1.0f, 0.0f, 0.0f }, { xSquare, ySquare + length, zSquare - length }, { 0.0f, 1.0f, 0.0f }, { xSquare, ySquare + length, zSquare }, { 0.0f, 0.0f, 1.0f },
		{ xSquare + length, ySquare + length, zSquare }, { 1.0f, 0.0f, 0.0f }, { xSquare + length, ySquare + length, zSquare - length }, { 0.0f, 1.0f, 0.0f }, { xSquare, ySquare + length, zSquare - length }, { 0.0f, 0.0f, 1.0f },

		{ xSquare + length, ySquare, zSquare }, { 1.0f, 0.0f, 0.0f }, { xSquare, ySquare, zSquare - length }, { 0.0f, 1.0f, 0.0f }, { xSquare, ySquare, zSquare }, { 0.0f, 0.0f, 1.0f },
		{ xSquare + length, ySquare, zSquare }, { 1.0f, 0.0f, 0.0f }, { xSquare + length, ySquare, zSquare - length }, { 0.0f, 1.0f, 0.0f }, { xSquare, ySquare, zSquare - length }, { 0.0f, 0.0f, 1.0f },

		{ xSquare + length, ySquare, zSquare }, { 1.0f, 0.0f, 0.0f }, { xSquare + length, ySquare + length, zSquare - length }, { 0.0f, 1.0f, 0.0f }, { xSquare + length, ySquare, zSquare - length }, { 0.0f, 0.0f, 1.0f },
		{ xSquare + length, ySquare, zSquare }, { 1.0f, 0.0f, 0.0f }, { xSquare + length, ySquare + length, zSquare }, { 0.0f, 1.0f, 0.0f }, { xSquare + length, ySquare + length, zSquare - length }, { 0.0f, 0.0f, 1.0f },

		{ xSquare, ySquare, zSquare }, { 1.0f, 0.0f, 0.0f }, { xSquare, ySquare + length, zSquare - length }, { 0.0f, 1.0f, 0.0f }, { xSquare, ySquare, zSquare - length }, { 0.0f, 0.0f, 1.0f },
		{ xSquare, ySquare, zSquare }, { 1.0f, 0.0f, 0.0f }, { xSquare, ySquare + length, zSquare }, { 0.0f, 1.0f, 0.0f }, { xSquare, ySquare + length, zSquare - length }, { 0.0f, 0.0f, 1.0f } };
	glGenVertexArrays(NumVAOs, VAOs);
	glGenBuffers(NumBuffers, Buffers);
	//===================================================
	// Square setup
	//===================================================
	glBindVertexArray(VAOs[Triangles]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Second value is 3 because our vertices conist of 3 values: x, y, z.
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(vPosition);
	// Set up the color vertex attribute pointer.
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(vColor);
	glBindVertexArray(0);


}

void display(void)
{

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glClearColor(0.0f, 0.0, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Define the camera.
	/*GLfloat viewpoint[3];
	viewpoint[0] = dist*sin(beta)*sin(alpha);
	viewpoint[1] = dist*cos(beta);
	viewpoint[2] = dist*sin(beta)*cos(alpha);
	glViewport(0, 0, width, height);
	glm::mat4 Projection = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 100.f);
	glm::mat4 View = glm::lookAt(glm::vec3(viewpoint[0], viewpoint[1], viewpoint[2]),
		glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, cos(beta - 3.141593*0.5), 0.0));*/
	glViewport(0, 0, width, height);
	glm::mat4 Projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.f);
	glm::mat4 view;
	glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(-0.2f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	view = glm::lookAt(
		cameraPos,
		cameraTarget,
		cameraUp);
	model = glm::mat4(1.0f);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));


	// Draw the square
	glBindVertexArray(VAOs[Triangles]);
	for (GLuint i = 0; i < 10; i++)
	{
		glVertexAttrib3f(2, 1, 0, 0);
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		GLfloat angle = 20.0f * i;
		model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(program, "model") , 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glFlush();
}
void reshape(int w, int h) {
	width = w;
	height = h;
}

// The viewport functions.//
//##########################################
void keyboard(unsigned char theKey, int mouseX, int mouseY) {
	GLint x = mouseX;
	GLint y = height - mouseY;
	switch (theKey)	{
	case 'v': dist -= DELTA; display(); break;
	case 'z': dist += DELTA; display(); break;
	case 'e': exit(-1);
	case 'd': glEnable(GL_DEPTH_TEST); display(); break;
	case 'f': glDisable(GL_DEPTH_TEST); display(); break;
	case 'b': glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);  display(); break;
	case 'n': glDisable(GL_BLEND); display(); break;
	case 't': glEnable(GL_STENCIL_TEST); display(); break;
	case 'y': glDisable(GL_STENCIL_TEST); display(); break;

	}
}

void special(int specKey, int mouseX, int mouseY) {
	GLint x = mouseX;
	GLint y = height - mouseY;
	switch (specKey) {
	case GLUT_KEY_LEFT: alpha -= DELTA; display(); break;
	case GLUT_KEY_RIGHT: alpha += DELTA; display(); break;
	case GLUT_KEY_UP: beta -= DELTA; display(); break;
	case GLUT_KEY_DOWN: display(); beta += DELTA; break;
	}
}

void mouse(int theButton, int State, int mouseX, int mouseY) {
	GLint x = mouseX; GLint y = height - mouseY;
}

void motion(int mouseX, int mouseY) {
	if (mouseX<(width / 2)) {
		alpha -= (mouseX - (width / 2)) / 10000.0;
	}
	else {
		alpha -= (mouseX - (width / 2)) / 10000.0;
	}
	if (mouseY<(height / 2)) {
		beta -= (mouseY - (height / 2)) / 10000.0;
	}
	else {
		beta -= (mouseY - (height / 2)) / 10000.0;
	}
	display();
}
//##########################################
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutInitContextVersion(4, 2);  // (4,5) (3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);  //GLUT_COMPATIBILITY_PROFILE
	glutCreateWindow(argv[0]);
	glewExperimental = GL_TRUE;
	if (glewInit()) printf("Error");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
}