// Vertices Shader - First
// GLSL (OpenGL Shading Language) 
#version 400

// Input -> VertexAttribPointers
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec2 in_Texture;
layout(location = 3) in vec3 in_Normal;

// Output (to the Fragment Shader)
out vec3 ex_Position;
out vec3 ex_Color;
out vec2 ex_Texture;
out vec3 ex_Normal;

// Uniforms
uniform mat4 ModelMatrix; // Local space -> World space transformations
uniform mat4 ViewMatrix; // World space -> View space transformations
uniform mat4 ProjectionMatrix; // View space -> Clip space transformations

void main(void)
{
    ex_Position = vec4(ModelMatrix * vec4(in_Position, 1.)).xyz;
    ex_Color = in_Color;
    ex_Texture = vec2(in_Texture.x, 1 - in_Texture.y); // Texture is flipped on Oy when read
    ex_Normal = mat3(transpose(inverse(ModelMatrix))) * in_Normal; // Normal moves when vertex moves

//    gl_Position = ModelMatrix * vec4(in_Position, 1.0f); // Output of the Vertex Shader
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_Position, 1.);
} 
 