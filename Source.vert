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

void main(void)
{
    ex_Position = in_Position;
    ex_Color = in_Color;
    ex_Texture = vec2(in_Texture.x, 1 - in_Texture.y);

    gl_Position = vec4(in_Position, 1.0f); // Output of the Vertex Shader
} 
 