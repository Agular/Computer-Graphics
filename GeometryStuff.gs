
#version 420 core

in VertexData {
    vec4 Color;
	vec2 zTex;
} VertexIn[];

out VertexData {
    vec4 Color;
	vec2 zTex;
} VertexOut;

//layout (triangles) in;
//layout (triangle_strip, max_vertices = 20) out;

//void main() {}

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
void main() {
	int i;
	for (i = 0; i < gl_in.length(); i++) {
		if(gl_PrimitiveIDIn%1==0){
			gl_Position = gl_in[i].gl_Position;
			VertexOut.Color=VertexIn[i].Color;
			VertexOut.zTex=VertexIn[i].zTex;
			EmitVertex();
		}
	}
	EndPrimitive();}
