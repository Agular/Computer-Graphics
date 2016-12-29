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
GLuint loadShaders(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath);
GLint height = 768, width = 1366;
GLuint Shader;
# define PI 3.14159265358979323846  /* pi */
GLfloat alpha = 0.2, beta = 0.8, dist = 5, DELTA = 0.5;
GLuint VAO, VBO, qVAO, qVBO;
GLuint texture1;
GLfloat vertices[];
//Model
glm::vec3 cubePositions[] = {
	glm::vec3(-1.0f, 1.0f, -1.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 1.0f)
};
void init(void)
{
	printf("\n%s", (char*)glGetString(GL_RENDERER));
	printf("\n%s", (char*)glGetString(GL_VERSION));
	printf("\n%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	Shader = loadShaders("LichtUndSchatten.vs", "LichtUndSchatten.fs", "");

	GLfloat vertices[] = {
		// Cube vertices, color vertices,  tex vertices vx, vy, vz, cx, cy, cz tx, ty, nx, ny, nz (normals)
		//BackFace - pink
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0, 0.0,-1.0,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0, 0.0, -1.0,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0, 0.0, -1.0,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0, 0.0, -1.0,
		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0, -1.0,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0, 0.0, -1.0,
		// Frontface - normal
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0, 0.0, 1.0,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0, 0.0, 1.0,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0, 0.0, 1.0,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0, 0.0, 1.0,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0, 1.0,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0, 0.0, 1.0,
		// Leftside - blue
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0, 0.0, 0.0,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0, 0.0, 0.0,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0, 0.0, 0.0,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0, 0.0, 0.0,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0, 0.0, 0.0,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0, 0.0, 0.0,
		// Rightside - red
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.0, 0.0,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0, 0.0, 0.0,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0, 0.0, 0.0,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0, 0.0, 0.0,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.0, 0.0,
		// Below - lightblue
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0, -1.0, 0.0,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0, -1.0, 0.0,
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0, -1.0, 0.0,
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0, -1.0, 0.0,
		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0, -1.0, 0.0,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0, -1.0, 0.0,
		// Up 
		-0.5f, 0.5f, -0.5f, 0.8f, 0.8f, 0.2f, 0.0f, 1.0f, 0.0, 1.0, 0.0,
		0.5f, 0.5f, -0.5f, 0.8f, 0.8f, 0.2f, 1.0f, 1.0f, 0.0, 1.0, 0.0,
		0.5f, 0.5f, 0.5f, 0.8f, 0.8f, 0.2f, 1.0f, 0.0f, 0.0, 1.0, 0.0,
		0.5f, 0.5f, 0.5f, 0.8f, 0.8f, 0.2f, 1.0f, 0.0f, 0.0, 1.0, 0.0,
		-0.5f, 0.5f, 0.5f, 0.8f, 0.8f, 0.2f, 0.0f, 0.0f, 0.0, 1.0, 0.0,
		-0.5f, 0.5f, -0.5f, 0.8f, 0.8f, 0.2f, 0.0f, 1.0f, 0.0, 1.0, 0.0
	};
	//===================================
	// Cube setup
	//===================================
	glGenVertexArrays(1, &qVAO);
	glGenBuffers(1, &qVBO);
	glBindVertexArray(qVAO);
	glBindBuffer(GL_ARRAY_BUFFER, qVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	//===================================
	// Cube setup
	//===================================
	FreeImage_Initialise(TRUE);
	FIBITMAP *bitmapData;
	int bitmapWidth, bitmapHeight;
	BYTE *bimapBits;
	FREE_IMAGE_FORMAT bitmapFormat = FIF_UNKNOWN;

	bitmapFormat = FreeImage_GetFileType("Bild01.bmp");
	bitmapData = FreeImage_Load(bitmapFormat, "Bild01.bmp");
	bitmapWidth = FreeImage_GetWidth(bitmapData);
	bitmapHeight = FreeImage_GetHeight(bitmapData);
	bimapBits = FreeImage_GetBits(bitmapData);

	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmapWidth, bitmapHeight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapWidth, bitmapHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bimapBits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);
	FreeImage_Unload(bitmapData);
	//===================================
	// Light variables
	//===================================

}

void display(void)
{

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, texture1);

	//===================================
	// Camera setup
	//===================================
	GLfloat viewpoint[3];
	viewpoint[0] = dist*sin(beta)*sin(alpha);
	viewpoint[1] = dist*cos(beta);
	viewpoint[2] = dist*sin(beta)*cos(alpha);
	glViewport(0, 0, width, height);
	glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);
	glm::mat4 View = glm::lookAt(glm::vec3(viewpoint[0], viewpoint[1], viewpoint[2]),
		glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, cos(beta - 3.141593*0.5), 0.0));
	glUseProgram(Shader);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "view"), 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
	
	//===================================
	// Light setup
	//===================================
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPosition = glm::vec3(0.0f, 5.0f, 0.0f);
	GLfloat shineDamper = 1.0;
	GLfloat reflectivity = 0.5;
	glUniform3f(glGetUniformLocation(Shader, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(Shader, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform1f(glGetUniformLocation(Shader, "shineDamper"), shineDamper);
	glUniform1f(glGetUniformLocation(Shader, "reflectivity"), reflectivity);
	//===================================
	// Draw the cubes
	//===================================
	for (GLuint i = 0; i < 10; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		GLfloat angle = 20.0f * i;
		model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glFlush();
}
void reshape(int w, int h) {
	width = w;
	height = h;
}

//===================================
// Keyboard settings
//===================================
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
//===================================
// Mouse setup
//===================================
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
//===================================
// Main loop
//===================================
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