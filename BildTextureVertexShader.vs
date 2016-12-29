
#version 420 core  // 450, 330 core , compatibility
uniform float mousePosX;
uniform float mousePosY;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTex;
out vec4 Color;
out vec2 vs_tex;

void main(){

	vs_tex = vTex;
	gl_Position = projection * view * model * vPosition;
	Color = vColor;
}
