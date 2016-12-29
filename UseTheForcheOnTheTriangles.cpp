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
# define PI           3.14159265358979323846  /* pi */
static float deltaX = 0.0;
GLuint loadShaders(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath);
GLint height = 512, width = 512;
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition, vColor };
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 27;
GLuint program;
GLint mposx = 512;
GLint mposy = 512;



void init(void)
{
	printf("\n%s", (char*)glGetString(GL_RENDERER));
	printf("\n%s", (char*)glGetString(GL_VERSION));
	printf("\n%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	program = loadShaders("TheForceVertexShader.vs", "TheForceFragmentShader.fs", "");
	glUseProgram(program);
	
	float x = -0.5;
	float y = 0.5;
	GLfloat vertices[NumVertices][2];
	float triangleLength = 0.2;
	
	for (int i = 0; i < 27; i+=3){
			vertices[i][0] = x;
			vertices[i][1] = y;
			vertices[i+1][0] = x - triangleLength;
			vertices[i + 1][1] = y;
			vertices[i + 2][0] = x - triangleLength;
			vertices[i + 2][1] = y + triangleLength;
			if (x == 0.5){
				x = -x;
				y -= 0.5;
			} else{
				x += 0.5;
			}
	}/*
	vertices[0][0] = -1.0f;
	vertices[0][1] = 1.0f;
	vertices[1][0] = -1.0f;
	vertices[1][1] = -1.0f;
	vertices[2][0] = 1.0f;
	vertices[2][1] = -1.0f;
	vertices[3][0] = -1.0f;
	vertices[3][1] = 1.0f;
	vertices[4][0] = 1.0f;
	vertices[4][1] = -1.0f;
	vertices[5][0] = 1.0f;
	vertices[5][1] = 1.0f;*/
	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(vPosition);

	// Send the screen width and height parameters to the shaders.
	GLfloat transformScreenWidth = glGetUniformLocation(program, "screenWidth");
	glUniform1f(transformScreenWidth, width);
	GLfloat transformScreenHeight = glGetUniformLocation(program, "screenHeight");
	glUniform1f(transformScreenHeight, height);


}



void display(void)
{
	//Send the user mouse coordinates to the shaders.
	GLfloat transformLocX = glGetUniformLocation(program, "mPosX");
	glUniform1f(transformLocX, mposx);
	GLfloat transformLocY = glGetUniformLocation(program, "mPosY");
	glUniform1f(transformLocY, mposy);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAOs[Triangles]);
	glVertexAttrib3f(vColor, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glFlush();
}
void reshape(int w, int h) {
	width = w;
	height = h;
}

void mouse(int theButton, int State, int mouseX, int mouseY) {
	if ((theButton == GLUT_LEFT_BUTTON) && (State == GLUT_DOWN)) {
		mposx = mouseX; mposy = height - mouseY;
		printf("%d,%d\n", mposx, mposy);
		glutPostRedisplay();

	}
}
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
	glutMouseFunc(mouse);
	glutMainLoop();
}
