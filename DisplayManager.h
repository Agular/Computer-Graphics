#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <glew.h>
#include <freeglut.h>
#include <FreeImage.h>

class DisplayManager {
	const int WIDTH = 1366;
	const int HEIGHT = 768;
public:
	void createDisplay(int argc, char** argv);
	void updateDisplay();
	void closeDisplay();
	bool shouldClose();
	void static display();
	
};