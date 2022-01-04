// Vertices Shader - First
// GLSL (OpenGL Shading Language) 
#version 400

// Input -> VertexAttribPointers
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec2 in_Texture;

// Output (to the Fragment Shader)
out vec3 ex_Position;
out vec3 ex_Color;
out vec2 ex_Texture;

// Uniforms
uniform mat4 ModelMatrix; // Matrix containing rotations, translations and scaling

void main(void)
{
    ex_Position = vec4(ModelMatrix * vec4(in_Position, 1.)).xyz;
    ex_Color = in_Color;
    ex_Texture = vec2(in_Texture.x, 1 - in_Texture.y); // Texture is flipped on Oy when read

    gl_Position = ModelMatrix * vec4(in_Position, 1.0f); // Output of the Vertex Shader
} 
 