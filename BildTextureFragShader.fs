
#version 420 core  // 450, 330 core , compatibility
uniform sampler2D tex;
uniform float mousePosX;
uniform float mousePosY;

in vec4 Color;
in vec2 vs_tex;
layout(location=0) out vec4 fColor;

void main() {

 fColor = texture(tex, vs_tex);
 }
