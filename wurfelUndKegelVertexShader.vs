
#version 420 core  // 450, 330 core , compatibility
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
out vec4 Color;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){ 
 gl_Position = projection * view * model * vPosition;
 Color = vColor;
  }
