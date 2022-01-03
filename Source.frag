// Fragment Shader - Second
// GLSL (OpenGL Shading Language) 
#version 400

// Input Values (from Vertex Shader)
in vec3 ex_Position;
in vec3 ex_Color;
in vec2 ex_Texture;

// Output values
out vec4 out_Color;


void main(void)
{
	out_Color = vec4(ex_Color, 1.f); // 4th coordinate -> alpha/opacity
}