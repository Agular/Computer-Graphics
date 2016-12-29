
#version 420 core  // 450, 330 core , compatibility
in vec4 Color;
out vec4 fColor;

// Screen width and height
uniform float screenHeight;
uniform float screenWidth;

// User mouse pointer xCoord and yCoord
uniform float mPosX;
uniform float mPosY;

// Converting (x,y,z) to range [0,1]
float x = gl_FragCoord.x/screenWidth;
float y = gl_FragCoord.y/screenHeight;

float ux = mPosX/screenWidth;
float uy = mPosY/screenHeight;  

// Converting from range [0,1] to NDC [-1,1]
float ndcx = x * 2.0 - 1.0;
float ndcy = y * 2.0 - 1.0;

float udcx = ux * 2.0 - 1.0;
float udcy = uy * 2.0 - 1.0;


vec2 fragVec = vec2(ndcx,ndcy);
vec2 userVec = vec2(udcx,udcy);

float dx = distance(ndcx,udcx);
float dy = distance(ndcy,udcy);
float d = distance(userVec,fragVec);
void main() {
 //fColor = vec4(1.0f*d,0.0f,0.0f,0.0f); 
 fColor = vec4(dx*0.5,dy*0.5,0.0f,0.0f); 
 }
