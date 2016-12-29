
#version 420 core  // 450, 330 core , compatibility
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec2 vTex;
layout(location = 3) in vec3 vNormal;

out vec4 Color;
out vec2 TexCoord;
out vec3 Normal;
out vec3 toLightVector;
out vec3 toCameraVector;

void main(){

	vec4 worldPosition = model * vec4(vPosition,1.0f);
	gl_Position = projection * view * worldPosition;
	Color = vec4(vColor, 1.0f);
	TexCoord = vTex;
	Normal = vNormal;
	toLightVector = lightPosition - worldPosition.xyz;
	toCameraVector = (inverse(view) * vec4(0.0f,0.0f,0.0f,1.0f)).xyz - worldPosition.xyz;
}
