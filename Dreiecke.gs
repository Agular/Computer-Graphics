
#version 420 core

in VertexData {
    vec4 Color;
} VertexIn[];

out VertexData {
    vec4 Color;
} VertexOut;

//layout (triangles) in;
//layout (triangle_strip, max_vertices = 20) out;

//void main() {}

layout (triangles) in;
layout (triangles) out;
void main() {
int i;
for (i = 0; i < gl_in.length(); i++)
{VertexOut.Color = VertexIn[i].Color;
EmitVertex();}
EndPrimitive();}
