/*
Beleg
Author: Ragnar Luga
*/


#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <glew.h>
#include <freeglut.h>
#include <FreeImage.h>
GLuint loadShaders(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath);
void setupTexture(GLuint TextureVar, char* textureName);
GLfloat height = 768, width = 1366;
GLuint Shader;
# define PI 3.14159265358979323846  /* pi */
GLfloat alpha = 0.2, beta = 0.8, dist = 5, DELTA = 0.5;
GLuint qVAO, qVBO, qTexture;
GLuint cubeVAO, cubeVBO, cubeTexture;

void init(void)
{
	glutSetWindowTitle("Christmas Tree");
	printf("Author: Ragnar Luga\n\n");
	printf("\n%s", (char*)glGetString(GL_RENDERER));
	printf("\n%s", (char*)glGetString(GL_VERSION));
	printf("\n%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	Shader = loadShaders("beleg.vs", "beleg.fs", "");

	GLfloat qVertices[] = { -0.5,0,-0.5, -0.5,0,0.5, 0.5,0,0.5, -0.5,0,-0.5, 0.5,0,0.5, 0.5,0,-0.5 };
	GLfloat qColors[] = { 0.5, 0.8, 0.7, 0.5, 0.8, 0.7, 0.5, 0.8, 0.7, 0.5, 0.8, 0.7, 0.5, 0.8, 0.7, 0.5, 0.8, 0.7 };
	GLfloat qNormals[] = { 0,1,0, 0,1,0, 0,1,0, 0,1,0, 0,1,0, 0,1,0 };
	GLfloat qTexs[] = { 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1 };
	GLint nQVertices = sizeof(qVertices) / sizeof(qVertices[0]);
	GLint nQColors = sizeof(qColors) / sizeof(qColors[0]);
	GLint nQNormals = sizeof(qNormals) / sizeof(qNormals[0]);
	GLint nQTexs = sizeof(qTexs) / sizeof(qTexs[0]);

	//===================================
	// Terrain setup
	//===================================
	glGenVertexArrays(1, &qVAO);
	glGenBuffers(1, &qVBO);
	glBindVertexArray(qVAO);
	glBindBuffer(GL_ARRAY_BUFFER, qVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(qVertices) + sizeof(qColors) + sizeof(qTexs) + sizeof(qNormals), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(qVertices), qVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(qVertices), sizeof(qColors), qColors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(qVertices) + sizeof(qColors), sizeof(qTexs), qTexs);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(qVertices) + sizeof(qColors) + sizeof(qTexs), sizeof(qNormals), qNormals);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)((nQVertices) * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)((nQVertices + nQColors) * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)((nQVertices + nQColors + nQTexs) * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	//===================================
	// Giftbox setup
	//===================================
	GLfloat cubeVertices[] = {
		//Front
		0.0,0.5,0.0, 0.0,0.0,0.0, 0.5,0,0.0, 0.0,0.5,0.0, 0.5,0,0.0, 0.5,0.5,0.0,
		//Back
		0.0,0.5,-0.5, 0.0,0.0,-0.5, 0.5,0,-0.5, 0.0,0.5,-0.5, 0.5,0,-0.5, 0.5,0.5,-0.5,
		//Right
		0.5,0.5,0.0, 0.5,0.0,0.0, 0.5,0.0,-0.5, 0.5,0.5,0.0, 0.5,0.0,-0.5, 0.5,0.5,-0.5,
		//Left
		0.0,0.5,0.0, 0.0,0.0,0.0, 0.0,0.0,-0.5, 0.0,0.5,0.0, 0.0,0.0,-0.5, 0.0,0.5,-0.5,
		//Low
		0.0,0.0,-0.5, 0.0,0.0,0.0, 0.5,0.0,0.0, 0.0,0.0,-0.5, 0.5,0.0,0.0, 0.5,0.0,-0.5,
		//High
		0.0,0.5,-0.5, 0.0,0.5,0.0, 0.5,0.5,0.0, 0.0,0.5,-0.5, 0.5,0.5,0.0, 0.5,0.5,-0.5

	};
	GLfloat cubeColors[] = { 
		//Front
		1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0,
		//Back
		1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0,
		//Right
		1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0,
		//Left
		1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0,
		//Low
		1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0,
		//High
		1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0
	};
	GLfloat cubeNormals[] = {
		//Front
		0,0,1, 0,0,1, 0,0,1, 0,0,1, 0,0,1, 0,0,1,
		//Back
		0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1,
		//Right
		1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0,
		//Left
		-1,0,0, -1,0,0, -1,0,0, -1,0,0, -1,0,0, -1,0,0,
		//Low
		0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0,
		//High
		0,1,0, 0,1,0, 0,1,0, 0,1,0, 0,1,0, 0,1,0
	};
	GLfloat cubeTexs[] = {
		//Front
		0,1, 0,0, 1,0, 0,1, 1,0, 1,1,
		//Back
		0,1, 0,0, 1,0, 0,1, 1,0, 1,1,
		//Right
		0,1, 0,0, 1,0, 0,1, 1,0, 1,1,
		//Left
		0,1, 0,0, 1,0, 0,1, 1,0, 1,1,
		//Low
		0,1, 0,0, 1,0, 0,1, 1,0, 1,1,
		//High
		//Low
		0,1, 0,0, 1,0, 0,1, 1,0, 1,1
	};
	GLint nCubeVertices = sizeof(cubeVertices) / sizeof(cubeVertices[0]);
	GLint nCubeColors = sizeof(cubeColors) / sizeof(cubeColors[0]);
	GLint nCubeNormals = sizeof(cubeNormals) / sizeof(cubeNormals[0]);
	GLint nCubeTexs = sizeof(cubeTexs) / sizeof(cubeTexs[0]);

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices) + sizeof(cubeColors) + sizeof(cubeTexs) + sizeof(cubeNormals), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertices), cubeVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertices), sizeof(cubeColors), cubeColors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertices) + sizeof(cubeColors), sizeof(cubeTexs), cubeTexs);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertices) + sizeof(cubeColors) + sizeof(cubeTexs), sizeof(cubeNormals), cubeNormals);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)((nCubeVertices) * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)((nCubeVertices + nCubeColors) * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)((nCubeVertices + nCubeColors + nCubeTexs) * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);
	//===================================
	// Textures setup
	//===================================
	FreeImage_Initialise(TRUE);
	FIBITMAP *bitmapData;
	int bitmapWidth, bitmapHeight;
	BYTE *bimapBits;
	FREE_IMAGE_FORMAT bitmapFormat = FIF_UNKNOWN;

	bitmapFormat = FreeImage_GetFileType("snow.jpg");
	bitmapData = FreeImage_Load(bitmapFormat, "snow.jpg");
	bitmapWidth = FreeImage_GetWidth(bitmapData);
	bitmapHeight = FreeImage_GetHeight(bitmapData);
	bimapBits = FreeImage_GetBits(bitmapData);

	glGenTextures(1, &qTexture);
	glBindTexture(GL_TEXTURE_2D, qTexture);
	printf("%d %d", bitmapHeight, bitmapWidth);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmapWidth, bitmapHeight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapWidth, bitmapHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bimapBits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	FreeImage_Unload(bitmapData);

	bitmapFormat = FreeImage_GetFileType("gift.jpg");
	bitmapData = FreeImage_Load(bitmapFormat, "gift.jpg");
	bitmapWidth = FreeImage_GetWidth(bitmapData);
	bitmapHeight = FreeImage_GetHeight(bitmapData);
	bimapBits = FreeImage_GetBits(bitmapData);

	glGenTextures(1, &cubeTexture);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	printf("%d %d", bitmapHeight, bitmapWidth);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmapWidth, bitmapHeight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapWidth, bitmapHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bimapBits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	FreeImage_Unload(bitmapData);

}

void display(void)
{

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

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
	glm::vec3 lightPosition = glm::vec3(0.0f, 5.0f, 5.0f);
	GLfloat shineDamper = 1.0;
	GLfloat reflectivity = 0.01;
	glUniform3f(glGetUniformLocation(Shader, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(Shader, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform1f(glGetUniformLocation(Shader, "shineDamper"), shineDamper);
	glUniform1f(glGetUniformLocation(Shader, "reflectivity"), reflectivity);


	//===================================
	// Draw the stuff.
	//===================================
	
	glBindTexture(GL_TEXTURE_2D, qTexture);
	glBindVertexArray(qVAO);
	glm::mat4 model;
	glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFlush();

	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	glBindVertexArray(cubeVAO);
	//mat4 model;
	glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
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
	switch (theKey) {
	case 'v': dist -= DELTA; display(); break;
	case 'z': dist += DELTA; display(); break;
	case 'e': exit(-1);
	case 'd': glEnable(GL_DEPTH_TEST); display(); break;
	case 'f': glDisable(GL_DEPTH_TEST); display(); break;
	case 'b': glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);  display(); break;
	case 'n': glDisable(GL_BLEND); display(); break;
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
	if (mouseX < (width / 2)) {
		alpha -= (mouseX - (width / 2)) / 10000.0;
	}
	else {
		alpha -= (mouseX - (width / 2)) / 10000.0;
	}
	if (mouseY < (height / 2)) {
		beta -= (mouseY - (height / 2)) / 10000.0;
	}
	else {
		beta -= (mouseY - (height / 2)) / 10000.0;
	}
	display();
}
void setupTexture(GLuint textureVar, char* textureName) {
	FreeImage_Initialise(TRUE);
	FIBITMAP *bitmapData;
	int bitmapWidth, bitmapHeight;
	BYTE *bimapBits;
	FREE_IMAGE_FORMAT bitmapFormat = FIF_UNKNOWN;

	bitmapFormat = FreeImage_GetFileType(textureName);
	bitmapData = FreeImage_Load(bitmapFormat, textureName);
	bitmapWidth = FreeImage_GetWidth(bitmapData);
	bitmapHeight = FreeImage_GetHeight(bitmapData);
	bimapBits = FreeImage_GetBits(bitmapData);

	glGenTextures(1, &textureVar);
	glBindTexture(GL_TEXTURE_2D, textureVar);
	printf("%d %d", bitmapHeight, bitmapWidth);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmapWidth, bitmapHeight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapWidth, bitmapHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bimapBits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//FreeImage_Unload(bitmapData);

}
//===================================
// Main loop
//===================================
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA| GLUT_MULTISAMPLE);
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