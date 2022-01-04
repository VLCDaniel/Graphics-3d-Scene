// Fragment Shader - Second
// GLSL (OpenGL Shading Language) 
#version 400

// Input Values (from Vertex Shader)
in vec3 ex_Position;
in vec3 ex_Color;
in vec2 ex_Texture;

// Output values
out vec4 out_Color;

// uniform = constant, each uniform has an id
uniform sampler2D texture0;
uniform sampler2D texture1;

void main(void)
{
	// Basic Color (Specified in Vertex)
//	out_Color = vec4(ex_Color, 1.f); // 4th coordinate -> alpha/opacity

	// Textures
//	out_Color = texture(texture0, ex_Texture); // texture0 -> first image, ex_Texture -> texture indices
//	out_Color = texture(texture1, ex_Texture); // second texture

	// Texture combinations
//	out_Color = texture(texture0, ex_Texture) * texture(texture1, ex_Texture);
//	out_Color = texture(texture0, ex_Texture) + texture(texture1, ex_Texture);
//	 mix two textures with 0.4 opacity (1.0 opacity -> displays only texture 2)
	out_Color = mix(texture(texture0, ex_Texture), texture(texture1, ex_Texture), 0.4);
}