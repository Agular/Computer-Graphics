
#version 420 core  // 450, 330 core , compatibility
uniform float mousePosX;
uniform float mousePosY;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTex;
layout(location = 3) in vec3 normal;
out vec4 Color;
out vec2 vs_tex;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;

out VertexData{
	vec4 Color;
} VertexOut;


void main(){

	vs_tex = vTex;
	vec4 worldPosition = model * vPosition;
	gl_Position = projection * view * worldPosition;
	Color = vColor;
	VertexOut.Color = vColor;

	surfaceNormal = normal;
	toLightVector = lightPosition - worldPosition.xyz;
	toCameraVector = (inverse(view) * vec4(0.0f,0.0f,0.0f,1.0f)).xyz - worldPosition.xyz;

}
