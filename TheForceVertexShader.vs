
#version 420 core  // 450, 330 core , compatibility
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
out vec4 Color;

// Screen width and height
uniform float screenHeight;
uniform float screenWidth;

// User mouse pointer xCoord and yCoord
uniform float mPosX;
uniform float mPosY;

float ux = mPosX/screenWidth;
float uy = mPosY/screenHeight;
float udcx = ux * 2.0 - 1.0;
float udcy = uy * 2.0 - 1.0;


vec4 vUser = vec4(udcx,udcy,0.0f,1.0f);
vec4 vDirection = vPosition - vUser;
float l = length(vDirection);

void main(){ 
 gl_Position = vPosition + (0.15f/l)*vDirection;

 Color = vColor;
  }
