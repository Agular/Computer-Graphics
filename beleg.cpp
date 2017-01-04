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
GLfloat height = 768, width = 1366;
GLuint Shader;
# define PI 3.14159265358979323846  /* pi */
GLfloat alpha = 0.2, beta = 0.8, dist = 5, DELTA = 0.5;
GLuint qVAO, qVBO, qTexture;

GLuint cubeVAO, cubeVBO, cubeTexture;
GLuint coneVAO, coneVBO, coneTexture;
GLuint cilinderVAO, cilinderVBO, cilinderTexture;
GLuint starVAO, starVBO, starTexture;

glm::vec3 cubePositions[] = {
	glm::vec3(0.6f, 0.0f, 0.6f),
	glm::vec3(-0.6f, 0.0f, 0.6f),
	glm::vec3(-0.6f, 0.0f, -0.6f),
	glm::vec3(0.6f, 0.0f, -0.6f)
};
glm::vec3 conePositions[] = {
	glm::vec3(0.0f, 0.5f, 0.0f),
	glm::vec3(0.0f, 0.9f, 0.0f),
	glm::vec3(0.0f, 1.3f, 0.0f),
	glm::vec3(0.0f, 1.7f, 0.0f),
	glm::vec3(0.0f, 2.1f, 0.0f)
};
glm::vec3 coneScales[] = {
	glm::vec3(2.0f, 2.0f, 2.0f),
	glm::vec3(1.75f, 1.75f, 1.75f),
	glm::vec3(1.5f, 1.5, 1.5f),
	glm::vec3(1.25f, 1.25f, 1.25f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};
void init(void)
{
	glutSetWindowTitle("Christmas Tree");
	printf("Author: Ragnar Luga\n\n");
	printf("\n%s", (char*)glGetString(GL_RENDERER));
	printf("\n%s", (char*)glGetString(GL_VERSION));
	printf("\n%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	Shader = loadShaders("beleg.vs", "beleg.fs", "");

	GLfloat qVertices[] = { -0.5,-0.01,-0.5, -0.5,-0.01,0.5, 0.5,-0.01,0.5, -0.5,-0.01,-0.5, 0.5,-0.01,0.5, 0.5,-0.01,-0.5 };
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
		//High
		0,1, 0,0, 1,0, 0,1, 1,0, 1,1,
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
	// Cone setup
	//===================================
	float radius = 0.25;
	const int nSpokes = 40;
	// Coordinates for the center of the bottom surface of the cone.
	float xCone = 0.0;
	float yCone = 0.0;
	float zCone = 0.0;
	// X, Y, Z for the tip of the cone.
	float xTip = 0.0;
	float yTip = 0.7;
	float zTip = 0.0;


	GLfloat coneVertices[nSpokes * 18];
	int idx = 0;
	int jdx = nSpokes * 18 / 2;
	for (double i = PI / (nSpokes / 2); i <= 2 * PI; i += PI / (nSpokes / 2)) {

		//Vertices for cone bottom surface.
		coneVertices[idx] = sin(i - PI / (nSpokes / 2))*radius + xCone;
		coneVertices[idx + 1] = yCone;
		coneVertices[idx + 2] = cos(i - PI / (nSpokes / 2))*radius + zCone;

		coneVertices[idx + 3] = xCone;
		coneVertices[idx + 4] = yCone;
		coneVertices[idx + 5] = zCone;

		coneVertices[idx + 6] = sin(i)*radius + xCone;
		coneVertices[idx + 7] = yCone;
		coneVertices[idx + 8] = cos(i)*radius + zCone;

		// Vertices for cone facets.
		coneVertices[jdx] = sin(i - PI / (nSpokes / 2))*radius + xCone;
		coneVertices[jdx + 1] = yCone;
		coneVertices[jdx + 2] = cos(i - PI / (nSpokes / 2))*radius + zCone;

		coneVertices[jdx + 3] = xTip;
		coneVertices[jdx + 4] = yTip;
		coneVertices[jdx + 5] = zTip;

		coneVertices[jdx + 6] = sin(i)*radius + xCone;
		coneVertices[jdx + 7] = yCone;
		coneVertices[jdx + 8] = cos(i)*radius + zCone;

		idx += 9;
		jdx += 9;
	}

	//For colors.
	GLfloat coneColors[nSpokes * 18];
	for (int i = 0; i < nSpokes * 18; i += 3) {
		coneColors[i] = 0.1;			//Red Color
		coneColors[i + 1] = 0.8;		//Green Color
		coneColors[i + 2] = 0.3;		//Blue Color
	}

	//For textures
	GLfloat coneTexs[nSpokes * 12];
	for (int i = 0; i < nSpokes * 12; i += 6) {
		coneTexs[i] = 0.0;
		coneTexs[i + 1] = 0.0;
		coneTexs[i + 2] = 0.5;
		coneTexs[i + 3] = 1.0;
		coneTexs[i + 4] = 1.0;
		coneTexs[i + 5] = 0.0;
	}
	GLfloat coneNormals[nSpokes * 18];
	//For normals. THe first half of all cone vertices face downwards, so we just hard-assingn the normal value
	for (int i = 0; i < nSpokes * 18; i += 9) {
		glm::vec3 p3 = glm::vec3(coneVertices[i], coneVertices[i + 1], coneVertices[i + 2]);
		glm::vec3 p2 = glm::vec3(coneVertices[i + 3], coneVertices[i + 4], coneVertices[i + 5]);
		glm::vec3 p1 = glm::vec3(coneVertices[i + 6], coneVertices[i + 7], coneVertices[i + 8]);

		glm::vec3 U = p2 - p1;
		glm::vec3 V = p3 - p1;
		glm::vec3 N;

		N.x = U.y * V.z - U.z * V.y;
		N.y = U.z * V.x - U.x * V.z;
		N.z = U.x * V.y - U.y * V.x;
		coneNormals[i] = N.x;
		coneNormals[i + 1] = N.y;
		coneNormals[i + 2] = N.z;
		coneNormals[i + 3] = N.x;
		coneNormals[i + 4] = N.y;
		coneNormals[i + 5] = N.z;
		coneNormals[i + 6] = N.x;
		coneNormals[i + 7] = N.y;
		coneNormals[i + 8] = N.z;
	}

	GLint nConeVertices = sizeof(coneVertices) / sizeof(coneVertices[0]);
	GLint nConeColors = sizeof(coneColors) / sizeof(coneColors[0]);
	GLint nConeTexs = sizeof(coneTexs) / sizeof(coneTexs[0]);
	GLint nConeNormals = sizeof(coneNormals) / sizeof(coneNormals[0]);

	glGenVertexArrays(1, &coneVAO);
	glGenBuffers(1, &coneVBO);
	glBindVertexArray(coneVAO);
	glBindBuffer(GL_ARRAY_BUFFER, coneVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coneVertices) + sizeof(coneColors) + sizeof(coneTexs) + sizeof(coneNormals), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coneVertices), coneVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(coneVertices), sizeof(coneColors), coneColors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(coneVertices) + sizeof(coneColors), sizeof(coneTexs), coneTexs);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(coneVertices) + sizeof(coneColors) + sizeof(coneTexs), sizeof(coneNormals), coneNormals);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)((nConeVertices) * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)((nConeVertices + nConeColors) * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)((nConeVertices + nConeColors + nConeTexs) * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	
	//===================================
	// Cilinder setup
	//===================================

	float cilinderRadius = 0.15;
	const int nCilinderSpokes = 6;
	const GLint nCilinderVertices = nCilinderSpokes * 36;
	// Coordinates for the center of the bottom surface of the cone.
	float xCilinder = 0.0;
	float yCilinder = 0.0;
	float zCilinder = 0.0;
	//Height of the cone
	float height = 0.5;

	GLfloat cilinderVertices[nCilinderVertices];
	idx = 0;
	jdx = nCilinderVertices / 4;
	int mdx = nCilinderVertices / 2;
	for (double i = PI / (nCilinderSpokes / 2); i <= 2 * PI; i += PI / (nCilinderSpokes / 2)) {

		//Vertices for cone bottom surface.
		cilinderVertices[idx] = sin(i - PI / (nCilinderSpokes / 2))*cilinderRadius + xCilinder;
		cilinderVertices[idx + 1] = yCilinder;
		cilinderVertices[idx + 2] = cos(i - PI / (nCilinderSpokes / 2))*cilinderRadius + zCilinder;

		cilinderVertices[idx + 3] = xCilinder;
		cilinderVertices[idx + 4] = yCilinder;
		cilinderVertices[idx + 5] = zCilinder;

		cilinderVertices[idx + 6] = sin(i)*cilinderRadius + xCilinder;
		cilinderVertices[idx + 7] = yCilinder;
		cilinderVertices[idx + 8] = cos(i)*cilinderRadius + zCilinder;

		//Vertices for cone upper surface.
		cilinderVertices[jdx] = sin(i - PI / (nCilinderSpokes / 2))*cilinderRadius + xCilinder;
		cilinderVertices[jdx + 1] = yCilinder + height;
		cilinderVertices[jdx + 2] = cos(i - PI / (nCilinderSpokes / 2))*cilinderRadius + zCilinder;

		cilinderVertices[jdx + 3] = xCilinder;
		cilinderVertices[jdx + 4] = yCilinder + height;
		cilinderVertices[jdx + 5] = zCilinder;

		cilinderVertices[jdx + 6] = sin(i)*cilinderRadius + xCilinder;
		cilinderVertices[jdx + 7] = yCilinder + height;
		cilinderVertices[jdx + 8] = cos(i)*cilinderRadius + zCilinder;

		// Creating vertices for the sides of the cilinder.
		cilinderVertices[mdx + 0] = sin(i)*cilinderRadius + xCilinder;
		cilinderVertices[mdx + 1] = yCilinder + height;
		cilinderVertices[mdx + 2] = cos(i)*cilinderRadius + zCilinder;

		cilinderVertices[mdx + 3] = sin(i)*cilinderRadius + xCilinder;
		cilinderVertices[mdx + 4] = yCilinder;
		cilinderVertices[mdx + 5] = cos(i)*cilinderRadius + zCilinder;

		cilinderVertices[mdx + 6] = sin(i - PI / (nCilinderSpokes / 2))*cilinderRadius + xCilinder;
		cilinderVertices[mdx + 7] = yCilinder;
		cilinderVertices[mdx + 8] = cos(i - PI / (nCilinderSpokes / 2))*cilinderRadius + zCilinder;

		cilinderVertices[mdx + 9] = sin(i)*cilinderRadius + xCilinder;
		cilinderVertices[mdx + 10] = yCilinder + height;
		cilinderVertices[mdx + 11] = cos(i)*cilinderRadius + zCilinder;

		cilinderVertices[mdx + 12] = sin(i - PI / (nCilinderSpokes / 2))*cilinderRadius + xCilinder;
		cilinderVertices[mdx + 13] = yCilinder;
		cilinderVertices[mdx + 14] = cos(i - PI / (nCilinderSpokes / 2))*cilinderRadius + zCilinder;

		cilinderVertices[mdx + 15] = sin(i - PI / (nCilinderSpokes / 2))*cilinderRadius + xCilinder;
		cilinderVertices[mdx + 16] = yCilinder + height;
		cilinderVertices[mdx + 17] = cos(i - PI / (nCilinderSpokes / 2))*cilinderRadius + zCilinder;

		idx += 9;
		jdx += 9;
		mdx += 18;
	}
	
	//For colors.
	const int nCilinderColors = nCilinderVertices;
	GLfloat cilinderColors[nCilinderColors];
	for (int i = 0; i < nCilinderVertices; i += 3) {
		cilinderColors[i] = 0.25;			//Red Color
		cilinderColors[i + 1] = 0.27;		//Green Color
		cilinderColors[i + 2] = 0.07;		//Blue Color
	}
	
	//For textures
	GLfloat cilinderTexs[nCilinderSpokes * 24];
	for (int i = 0; i < nCilinderSpokes * 24; i += 6) {
		cilinderTexs[i] = 0.0;
		cilinderTexs[i + 1] = 0.0;
		cilinderTexs[i + 2] = 0.5;
		cilinderTexs[i + 3] = 1.0;
		cilinderTexs[i + 4] = 1.0;
		cilinderTexs[i + 5] = 0.0;
	}
	
	GLfloat cilinderNormals[nCilinderVertices];
	//For normals. THe first half of all cone vertices face downwards, so we just hard-assingn the normal value
	for (int i = 0; i < nCilinderVertices; i += 9) {
		glm::vec3 p3 = glm::vec3(cilinderVertices[i], cilinderVertices[i + 1], cilinderVertices[i + 2]);
		glm::vec3 p2 = glm::vec3(cilinderVertices[i + 3], cilinderVertices[i + 4], cilinderVertices[i + 5]);
		glm::vec3 p1 = glm::vec3(cilinderVertices[i + 6], cilinderVertices[i + 7], cilinderVertices[i + 8]);

		glm::vec3 U = p2 - p1;
		glm::vec3 V = p3 - p1;
		glm::vec3 N;

		N.x = U.y * V.z - U.z * V.y;
		N.y = U.z * V.x - U.x * V.z;
		N.z = U.x * V.y - U.y * V.x;
		cilinderNormals[i] = N.x;
		cilinderNormals[i + 1] = N.y;
		cilinderNormals[i + 2] = N.z;
		cilinderNormals[i + 3] = N.x;
		cilinderNormals[i + 4] = N.y;
		cilinderNormals[i + 5] = N.z;
		cilinderNormals[i + 6] = N.x;
		cilinderNormals[i + 7] = N.y;
		cilinderNormals[i + 8] = N.z;
	}
	
	GLint nCilinderTexs = sizeof(cilinderTexs) / sizeof(cilinderTexs[0]);
	GLint nCilinderNormals = sizeof(cilinderNormals) / sizeof(cilinderNormals[0]);
	
	glGenVertexArrays(1, &cilinderVAO);
	glGenBuffers(1, &cilinderVBO);
	glBindVertexArray(cilinderVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cilinderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cilinderVertices) + sizeof(cilinderColors) + sizeof(cilinderTexs) + sizeof(cilinderNormals), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cilinderVertices), cilinderVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cilinderVertices), sizeof(cilinderColors), cilinderColors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cilinderVertices) + sizeof(cilinderColors), sizeof(cilinderTexs), cilinderTexs);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cilinderVertices) + sizeof(cilinderColors) + sizeof(cilinderTexs), sizeof(cilinderNormals), cilinderNormals);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)((nCilinderVertices) * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)((nCilinderVertices + nCilinderColors) * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)((nCilinderVertices + nCilinderColors + nCilinderTexs) * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	/*
	float cilinderRadius = 0.15;
	const int nCilinderSpokes = 6;
	const GLint nCilinderVertices = nCilinderSpokes * 36;
	// Coordinates for the center of the bottom surface of the cone.
	float xCilinder = 0.0;
	float yCilinder = 0.0;
	float zCilinder = 0.0;
	//Height of the cone
	float height = 0.5;

	GLfloat cilinderVertices[nCilinderVertices];
	idx = 0;
	jdx = nCilinderVertices / 4;
	int mdx = nCilinderVertices / 2;
	for (double i = PI / (nCilinderSpokes / 2); i <= 2 * PI; i += PI / (nCilinderSpokes / 2)) {

		//Vertices for cone bottom surface.
		cilinderVertices[idx] = sin(i - PI / (nCilinderSpokes / 2))*cilinderRadius + xCilinder;
		cilinderVertices[idx + 1] = yCilinder;
		cilinderVertices[idx + 2] = cos(i - PI / (nCilinderSpokes / 2))*cilinderRadius + zCilinder;

		cilinderVertices[idx + 3] = xCilinder;
		cilinderVertices[idx + 4] = yCilinder;
		cilinderVertices[idx + 5] = zCilinder;

		cilinderVertices[idx + 6] = sin(i)*cilinderRadius + xCilinder;
		cilinderVertices[idx + 7] = yCilinder;
		cilinderVertices[idx + 8] = cos(i)*cilinderRadius + zCilinder;

		//Vertices for cone upper surface.
		cilinderVertices[jdx] = sin(i - PI / (nCilinderSpokes / 2))*cilinderRadius + xCilinder;
		cilinderVertices[jdx + 1] = yCilinder + height;
		cilinderVertices[jdx + 2] = cos(i - PI / (nCilinderSpokes / 2))*cilinderRadius + zCilinder;

		cilinderVertices[jdx + 3] = xCilinder;
		cilinderVertices[jdx + 4] = yCilinder + height;
		cilinderVertices[jdx + 5] = zCilinder;

		cilinderVertices[jdx + 6] = sin(i)*cilinderRadius + xCilinder;
		cilinderVertices[jdx + 7] = yCilinder + height;
		cilinderVertices[jdx + 8] = cos(i)*cilinderRadius + zCilinder;

		idx += 9;
		jdx += 9;
	}
	
	//For colors.
	const int nCilinderColors = nCilinderVertices;
	GLfloat cilinderColors[nCilinderColors];
	for (int i = 0; i < nCilinderVertices; i += 3) {
		cilinderColors[i] = 0.25;			//Red Color
		cilinderColors[i + 1] = 0.27;		//Green Color
		cilinderColors[i + 2] = 0.07;		//Blue Color
	}
	
	//For textures
	GLfloat cilinderTexs[nCilinderSpokes * 24];
	for (int i = 0; i < nCilinderSpokes * 24; i += 6) {
		cilinderTexs[i] = 0.0;
		cilinderTexs[i + 1] = 0.0;
		cilinderTexs[i + 2] = 0.5;
		cilinderTexs[i + 3] = 1.0;
		cilinderTexs[i + 4] = 1.0;
		cilinderTexs[i + 5] = 0.0;
	}
	
	GLfloat cilinderNormals[nCilinderVertices];
	//For normals. THe first half of all cone vertices face downwards, so we just hard-assingn the normal value
	for (int i = 0; i < nCilinderVertices; i += 9) {
		glm::vec3 p3 = glm::vec3(cilinderVertices[i], cilinderVertices[i + 1], cilinderVertices[i + 2]);
		glm::vec3 p2 = glm::vec3(cilinderVertices[i + 3], cilinderVertices[i + 4], cilinderVertices[i + 5]);
		glm::vec3 p1 = glm::vec3(cilinderVertices[i + 6], cilinderVertices[i + 7], cilinderVertices[i + 8]);

		glm::vec3 U = p2 - p1;
		glm::vec3 V = p3 - p1;
		glm::vec3 N;

		N.x = U.y * V.z - U.z * V.y;
		N.y = U.z * V.x - U.x * V.z;
		N.z = U.x * V.y - U.y * V.x;
		cilinderNormals[i] = N.x;
		cilinderNormals[i + 1] = N.y;
		cilinderNormals[i + 2] = N.z;
		cilinderNormals[i + 3] = N.x;
		cilinderNormals[i + 4] = N.y;
		cilinderNormals[i + 5] = N.z;
		cilinderNormals[i + 6] = N.x;
		cilinderNormals[i + 7] = N.y;
		cilinderNormals[i + 8] = N.z;
	}
	
	GLint nCilinderTexs = sizeof(cilinderTexs) / sizeof(cilinderTexs[0]);
	GLint nCilinderNormals = sizeof(cilinderNormals) / sizeof(cilinderNormals[0]);
	
	glGenVertexArrays(1, &cilinderVAO);
	glGenBuffers(1, &cilinderVBO);
	glBindVertexArray(cilinderVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cilinderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cilinderVertices) + sizeof(cilinderColors) + sizeof(cilinderTexs) + sizeof(cilinderNormals), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cilinderVertices), cilinderVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cilinderVertices), sizeof(cilinderColors), cilinderColors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cilinderVertices) + sizeof(cilinderColors), sizeof(cilinderTexs), cilinderTexs);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cilinderVertices) + sizeof(cilinderColors) + sizeof(cilinderTexs), sizeof(cilinderNormals), cilinderNormals);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)((nCilinderVertices) * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)((nCilinderVertices + nCilinderColors) * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)((nCilinderVertices + nCilinderColors + nCilinderTexs) * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);
	*/


	
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
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmapWidth, bitmapHeight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapWidth, bitmapHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bimapBits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	FreeImage_Unload(bitmapData);

	bitmapFormat = FreeImage_GetFileType("spruce.png");
	bitmapData = FreeImage_Load(bitmapFormat, "spruce.png");
	bitmapWidth = FreeImage_GetWidth(bitmapData);
	bitmapHeight = FreeImage_GetHeight(bitmapData);
	bimapBits = FreeImage_GetBits(bitmapData);

	glGenTextures(1, &coneTexture);
	glBindTexture(GL_TEXTURE_2D, coneTexture);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmapWidth, bitmapHeight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapWidth, bitmapHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bimapBits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	FreeImage_Unload(bitmapData);

	bitmapFormat = FreeImage_GetFileType("brown.png");
	bitmapData = FreeImage_Load(bitmapFormat, "brown.png");
	bitmapWidth = FreeImage_GetWidth(bitmapData);
	bitmapHeight = FreeImage_GetHeight(bitmapData);
	bimapBits = FreeImage_GetBits(bitmapData);

	glGenTextures(1, &cilinderTexture);
	glBindTexture(GL_TEXTURE_2D, cilinderTexture);
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
	glClearColor(0.1f, 0.2f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	//Draw the terrain quad.
	glBindTexture(GL_TEXTURE_2D, qTexture);
	glBindVertexArray(qVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));
	model = glm::scale(model, glm::vec3(3, 0, 3));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFlush();



	//Draw the cubes
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	glBindVertexArray(cubeVAO);
	for (GLuint i = 0; i < 4; i++)
	{
		model = glm::mat4(1.0);
		model = glm::translate(model, cubePositions[i]);
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glFlush();

	// Draw the cones
	model = glm::mat4(1.0);
	glBindTexture(GL_TEXTURE_2D, coneTexture);
	glBindVertexArray(coneVAO);
	for (GLuint i = 0; i < 5; i++)
	{
		model = glm::mat4(1.0);
		model = glm::translate(model, conePositions[i]);
		model = glm::scale(model, coneScales[i]);
		glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 240);
	}
	glFlush();

	glBindTexture(GL_TEXTURE_2D, cilinderTexture);
	glBindVertexArray(cilinderVAO);
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 72);
	glFlush();

	glBindVertexArray(0);
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
//===================================
// Main loop
//===================================
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
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