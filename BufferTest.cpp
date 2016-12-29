/*
  Dreiecke
*/

#include "BufferTest.h"

GLint height=100,width=100;
enum VAO_IDs {CompleteObject, NumVAOs};
enum Buffer_IDs {ObjectBuffer,NumBuffers};
enum Attrib_IDs {vPosition,vColor};
enum Uniform_IDs { mousePosXLocation, mousePosYLocation,NumUniforms};
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint Uniforms[NumUniforms];

const GLfloat sizeFactor = 1;
const GLfloat edgeLength = 1;

const char* vertexShader = "BufferTest.vs";
const char* fragmentShader = "BufferTest.fs";
GLuint program; 


GLfloat pi = 3.14159;
GLfloat alpha = 0.2, beta = 0.8, dist = 5, DELTA = 0.5;

bool wireFrame = false;
bool stencil = true;	
bool blend = true;

vec3 vertices[6] = { { 0.0,0.0,0.0 },{ 1.0,1.0,0.0 },{ 1.0,0.0,0.0 },{ 1.0,0.0,1.0 },{ 0.0,1.0,1.0 },{ 0.0,10.0,1.0 } };
vec4 colors[6] = { { 1.0,0.0,0.0,1.0 },{ 0.0,1.0,0.0,1.0 },{ 0.0,0.0,1.0,1.0 },{ 1.0,0.0,1.0,1.0 },{ 0.0,1.0,1.0,1.0 },{ 1.0,1.0,1.0,1.0 } };
vec2 texData[6] = { { 0.0,0.0 },{ 1.0,0.0 },{ 1.0,1.0 },{ 1.0,0.0 },{ 0.0,1.0 },{ 0.5,1.0 } };

static GLubyte checkImage[128][128][4];

static GLubyte randomImage[512][512][4];
int currentTexture = 4;

const int textureCount = 5;
GLuint tex[textureCount];
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(4, 2);  // (4,5) (3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);  //GLUT_COMPATIBILITY_PROFILE
	glutCreateWindow(argv[0]);
	glewExperimental = GL_TRUE;
	if (glewInit()) printf("Error: glewInit() failed");
	init();						// Initialize OpenGL
	glutReshapeFunc(reshape);		// Give glut our reshape callback. This will be called every 
									// time the user changes the size of the window
	glutDisplayFunc(display);		// Tell glut our 'render' method in order it can repaint the window
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();				// Okay, thats enough, just give our control to glut, it will do everything for us.
	
}

void init(void)
{
	// Console output to tell somne technical stuff
	// and what the purpose of the application is
	printf("Computergrafik 1 - Praktikum Aufgabe 2.1\nAutor: Raphael Pour\n");
	printf("\nOpenGL Renderer:\t%s", (char*)glGetString(GL_RENDERER));
	printf("\nOpenGL Version:\t\t%s", (char*)glGetString(GL_VERSION));
	printf("\nShader Language Version:%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Load shader with Prof.Oertels function
	program = loadShaders(vertexShader, fragmentShader, "");
	glUseProgram(program);

	// Define Uniforms for the shader and link the variables to it
	Uniforms[mousePosXLocation] = glGetUniformLocation(program, "mousePosX");
	Uniforms[mousePosYLocation] = glGetUniformLocation(program, "mousePosY");

	glUniform1f(Uniforms[mousePosXLocation], (GLfloat)height);
	glUniform1f(Uniforms[mousePosYLocation], (GLfloat)width);

	// Enable OpenGL stuff
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glGenBuffers(NumBuffers, Buffers);
	glGenVertexArrays(NumVAOs, VAOs);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(texData), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices),  sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices)+ sizeof(colors), sizeof(texData), texData);


	glBindVertexArray(VAOs[0]);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertices));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(colors)+ sizeof(vertices)));
	glEnableVertexAttribArray(2);



	// Texture Stuff
	makeCheckImage();
	glGenTextures(textureCount, tex);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 128, 128);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 128, 128);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);
	
	makeRandomImage();
	glBindTexture(GL_TEXTURE_2D, tex[2]);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, randomImage);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);

	// Import image
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

	glBindTexture(GL_TEXTURE_2D, tex[3]);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmapWidth, bitmapHeight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapWidth, bitmapHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bimapBits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);

	FreeImage_Unload(bitmapData);
	bitmapFormat = FreeImage_GetFileType("Bild02.bmp");
	bitmapData = FreeImage_Load(bitmapFormat, "Bild02.bmp");
	bitmapWidth = FreeImage_GetWidth(bitmapData);
	bitmapHeight = FreeImage_GetHeight(bitmapData);
	bimapBits = FreeImage_GetBits(bitmapData);

	glBindTexture(GL_TEXTURE_2D, tex[4]);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmapWidth, bitmapHeight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapWidth, bitmapHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bimapBits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);

	FreeImage_Unload(bitmapData);


	glClearColor(0, 0, 0, 0);		
}

void display(void)
{
	// Clear screen using our backgroundcolor (set in init())
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	glBindTexture(GL_TEXTURE_2D, tex[currentTexture]);

	// Code given by Prof. Oertel to make the scene interactive
	GLfloat viewpoint[3];
	viewpoint[0] = dist*sin(beta)*sin(alpha);
	viewpoint[1] = dist*cos(beta);
	viewpoint[2] = dist*sin(beta)*cos(alpha);
	glViewport(0, 0, width, height);
	glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);
	glm::mat4 View = glm::lookAt(glm::vec3(viewpoint[0], viewpoint[1], viewpoint[2]),
						glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, cos(beta - 3.141593*0.5), 0.0));
	glm::mat4 Model = glm::mat4(1.0f);
	//glm::mat4 ModelViewProjection = glm::mat4(1.0f);
	glm::mat4 ModelViewProjection = Projection*View*Model;
	GLint l = glGetUniformLocation(program, "ModelViewProjection");
	glUniformMatrix4fv(l, 1, GL_FALSE, &ModelViewProjection[0][0]);

	glBindVertexArray(VAOs[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

	//c->display();

	// Forces all OpenGL commands which are defined previously defined to execute
	glFlush();	
}

void reshape(int w, int h)
{
  width=w;
  height=h;


}




void keyboard(unsigned char theKey, int mouseX, int mouseY)
{
	GLint x = mouseX;
	GLint y = height - mouseY;
	switch (theKey) {
	case 'v': dist -= DELTA; display(); break;
	case 'z': dist += DELTA; display(); break;
	case 'e': exit(-1);
	case '1': currentTexture = 0; break;
	case '2': currentTexture = 1; break;
	case '3': currentTexture = 2; break;
	case '4': currentTexture = 3; break;
	case '5': currentTexture = 4; break;
	case '-': 
		wireFrame = !wireFrame; 
		glPolygonMode(GL_FRONT_AND_BACK, (wireFrame) ? (GL_LINE) : (GL_FILL));
		display();
		break;

	}
}

void special(int specKey, int mouseX, int mouseY) 
{
	GLint x = mouseX;
	GLint y = height - mouseY;
	switch (specKey) {
	case GLUT_KEY_LEFT: alpha -= DELTA; display(); break;
	case GLUT_KEY_RIGHT: alpha += DELTA; display(); break;
	case GLUT_KEY_UP: beta -= DELTA; display(); break;
	case GLUT_KEY_DOWN: display(); beta += DELTA; break;
	}
}

void mouse(int theButton, int State, int mouseX, int mouseY) 
{
	GLint x = mouseX; GLint y = height - mouseY;
}

void motion(int mouseX, int mouseY) 
{
	if (mouseX<(width / 2)) { alpha -= (mouseX - (width / 2)) / 10000.0; }
	else { alpha -= (mouseX - (width / 2)) / 10000.0; }
	if (mouseY<(height / 2)) { beta -= (mouseY - (height / 2)) / 10000.0; }
	else { beta -= (mouseY - (height / 2)) / 10000.0; }
	display();
}

void makeCheckImage(void)
{
	int i, j, c;
	for (i = 0; i<128; i++) {
		for (j = 0; j<128; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}

void makeRandomImage(void)
{
	std::random_device engine;
	for (int x = 0; x < 128; x++)
		for (int y = 0; y < 128; y++)
		{
			randomImage[x][y][0] = engine();
			randomImage[x][y][1] = engine();
			randomImage[x][y][2] = engine();
			randomImage[x][y][3] = engine();
		}
}

