/*
  Dreiecke
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
GLint height=100,width=100;
enum VAO_IDs {Triangles,NumVAOs};
enum Buffer_IDs {ArrayBuffer,NumBuffers};
enum Attrib_IDs {vPosition,vColor};
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices=200;
GLuint program; 

void init(void)
{printf("\n%s",(char*)glGetString(GL_RENDERER));
 printf("\n%s",(char*)glGetString( GL_VERSION));
 printf("\n%s\n",(char*)glGetString( GL_SHADING_LANGUAGE_VERSION));
 program = loadShaders("Dreiecke.vs", "Dreiecke.fs", "");
 glUseProgram(program);
 float radius = 0.5;

 GLfloat vertices[NumVertices][2];
 int idx = 0;
 int kidx = 40;
 int lidx = 41;
 for (double i = 0; i < 2 * PI; i += PI / 20){
	 vertices[idx][0] = sin(i)*radius;
	 vertices[idx][1] = cos(i)*radius;

	 vertices[kidx][0] = 0;
	 vertices[kidx][1] = 0;

	 vertices[lidx][0] = sin(i)*radius;
	 vertices[lidx][1] = cos(i)*radius;
	 idx += 1;
	 kidx += 2;
	 lidx += 2;
 }
 glGenBuffers(NumBuffers,Buffers);
 glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
 glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
 glGenVertexArrays(NumVAOs,VAOs);
 glBindVertexArray(VAOs[Triangles]);
 glVertexAttribPointer(vPosition,2,GL_FLOAT,GL_FALSE,0,(void*)0);
 glEnableVertexAttribArray(vPosition); }


void display(void)
{glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(VAOs[Triangles]);
  glVertexAttrib3f(vColor,1,0,0);
  glDrawArrays(GL_LINE_LOOP, 0,40);
  glDrawArrays(GL_LINES, 40, 81);

 glFlush();
 //glutSwapBuffers();
}
void reshape(int w, int h) {
  width=w;
  height=h; }

void timer(int v) {
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(deltaX, 0.0f, 0.0f));
	transform = glm::rotate(transform, (GLfloat)glutGet(GLUT_ELAPSED_TIME) * glm::radians(0.5f), glm::vec3(0.0f, 0.0f, 1.0f));

	// Get matrix's uniform location and set matrix
	GLint transformLoc = glGetUniformLocation(program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glutPostRedisplay();
	glFlush();
	deltaX += 0.007;
	glutTimerFunc(1000/60, timer, v);
}
int main(int argc, char** argv)
{glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_RGBA);
 glutInitWindowSize(512,512);
 glutInitContextVersion(4,2);  // (4,5) (3,3);
 glutInitContextProfile(GLUT_CORE_PROFILE);  //GLUT_COMPATIBILITY_PROFILE
 glutCreateWindow(argv[0]);
 glewExperimental=GL_TRUE;
 if (glewInit()) printf("Error");
 init();
 glutReshapeFunc(reshape);
 glutDisplayFunc(display);
 glutTimerFunc(100, timer, 0);
 glutMainLoop(); }
