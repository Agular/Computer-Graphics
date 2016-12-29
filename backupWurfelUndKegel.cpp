/*
UseTHeForceOnTheTriangles
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
GLuint loadShaders(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath);
GLint height = 512, width = 512;
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers};
enum Attrib_IDs { vPosition, vColor };
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint program;
const GLuint NumVertices = 36;
const GLuint nConeVertices = 200;
GLfloat pi = 3.14159;
# define PI           3.14159265358979323846  /* pi */
GLfloat alpha = 0.2, beta = 0.8, dist = 5, DELTA = 0.5;
GLuint VAO, VBO;
GLfloat ConeVertices[nConeVertices][3];
//Parameters for cone
float radius = 0.5;
int nSpeichen = 40;
float xCone;
float yCone;
float zCone;
//Parameters for square
float length = 0.5;
// X, Y, Z for the left-down front facing side.
float xSquare = -0.5;
float ySquare = 0.25;
float zSquare = 0.0;

void init(void)
{
	printf("\n%s", (char*)glGetString(GL_RENDERER));
	printf("\n%s", (char*)glGetString(GL_VERSION));
	printf("\n%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	program = loadShaders("wurfelUndKegelVertexShader.vs", "wurfelUndKegelFragShader.fs", "");
	glUseProgram(program);

	// My cube vertices. They start from front side.
	GLfloat vertices[NumVertices][3] = {
		{ xSquare + length, ySquare, zSquare }, { xSquare, ySquare + length, zSquare }, { xSquare, ySquare, zSquare },
		{ xSquare + length, ySquare, zSquare }, { xSquare + length, ySquare + length, zSquare }, { xSquare, ySquare + length, zSquare },

		{ xSquare + length, ySquare, zSquare - length }, { xSquare, ySquare + length, zSquare - length }, { xSquare, ySquare, zSquare - length },
		{ xSquare + length, ySquare, zSquare - length }, { xSquare + length, ySquare + length, zSquare - length }, { xSquare, ySquare + length, zSquare - length },
	
		{ xSquare + length, ySquare + length, zSquare }, { xSquare, ySquare + length, zSquare - length }, { xSquare, ySquare + length, zSquare },
		{ xSquare + length, ySquare + length, zSquare }, { xSquare + length, ySquare + length, zSquare - length }, { xSquare, ySquare + length, zSquare - length },

		{ xSquare + length, ySquare, zSquare }, { xSquare, ySquare, zSquare - length }, { xSquare, ySquare, zSquare },
		{ xSquare + length, ySquare, zSquare }, { xSquare + length, ySquare, zSquare - length }, { xSquare, ySquare, zSquare - length },
	
		{ xSquare + length, ySquare, zSquare }, { xSquare + length, ySquare + length, zSquare - length }, { xSquare + length, ySquare, zSquare - length },
		{ xSquare + length, ySquare, zSquare }, { xSquare + length, ySquare + length, zSquare }, { xSquare + length, ySquare + length, zSquare - length },

		{ xSquare, ySquare, zSquare }, { xSquare, ySquare + length, zSquare - length }, { xSquare, ySquare, zSquare - length },
		{ xSquare, ySquare, zSquare }, { xSquare, ySquare + length, zSquare }, { xSquare, ySquare + length, zSquare - length } };
	
	int idx = 0;
	int kidx = 40;
	int lidx = 41;
	for (double i = 0; i < 2 * PI; i += PI / (nSpeichen/2)){
		ConeVertices[idx][0] = sin(i)*radius + 0.5;
		ConeVertices[idx][1] = cos(i)*radius - 0.5;

		ConeVertices[kidx][0] = 0.5;
		ConeVertices[kidx][1] = -0.5;

		ConeVertices[lidx][0] = sin(i)*radius + 0.5;
		ConeVertices[lidx][1] = cos(i)*radius - 0.5;
		idx += 1;
		kidx += 2;
		lidx += 2;
	}


	glGenVertexArrays(NumVAOs, VAOs);
	glGenBuffers(NumBuffers, Buffers);
	//===================================================
	// Square setup
	//===================================================
	glBindVertexArray(VAOs[Triangles]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Second value is 3 because our vertices conist of 3 values: x, y, z.
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(vPosition);
	glBindVertexArray(0);
	//===================================================
	// Cone setup
	//===================================================
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ConeVertices),ConeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,0,(GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void display(void)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glClearColor(0.0f, 0.0, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Define the camera.
	GLfloat viewpoint[3];
	viewpoint[0] = dist*sin(beta)*sin(alpha);
	viewpoint[1] = dist*cos(beta);
	viewpoint[2] = dist*sin(beta)*cos(alpha);
	glViewport(0, 0, width, height);
	glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);
	glm::mat4 View = glm::lookAt(glm::vec3(viewpoint[0], viewpoint[1], viewpoint[2]),
	glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, cos(beta - 3.141593*0.5), 0.0));
	glm::mat4 Model = glm::mat4(1.0f);
	// This line below by the professor. XD troll.
	//glm::mat4 ModelViewProjection = glm::mat4(1.0f);
	glm::mat4 ModelViewProjection=Projection*View*Model;
	GLuint Location = glGetUniformLocation(program, "ModelViewProjection");
	glUniformMatrix4fv(Location, 1, GL_FALSE, &ModelViewProjection[0][0]);
	
	// Draw the square
	glBindVertexArray(VAOs[Triangles]);
	glVertexAttrib3f(vColor, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glFlush();
	// Draw the cone
	glVertexAttrib3f(vColor, 1, 1, 0);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_LOOP, 0, 40);
	glDrawArrays(GL_LINES, 40, 81);
	glFlush();
	glBindVertexArray(0);
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
