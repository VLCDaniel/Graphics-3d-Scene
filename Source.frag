// Fragment Shader - Second
// GLSL (OpenGL Shading Language) 
#version 400

// Input Values (from Vertex Shader)
in vec3 ex_Position;
in vec3 ex_Color;
in vec2 ex_Texture;
in vec3 ex_Normal;

// Output values
out vec4 out_Color;

// uniform = constant, each uniform has an id
uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 lightPos0;
uniform vec3 cameraPos;

vec3 lightColor = vec3(1.0, 1.0, 1.0); // white

void main(void)
{
	// PHONE LIGHTING

	// Ambient light -> there is always a small amout of light
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;


	// Diffuse light -> gives objects brightness the closer the fragments
	//					are aligned to the light rays

	// normalised direction from the light source to the normal
	vec3 lightDir = normalize(ex_Position - lightPos0);
	vec3 norm = normalize(ex_Normal); // make sure the normal is normalised

	float diff = max(dot(norm, lightDir), 0.0); // get angle between lightDir and norm
	vec3 diffuse = diff * lightColor; // get the diffuse component -> larger angle -> lower light


	// Specular light -> reflextion
	float specularStrength = 0.7; // intensity value

	vec3 viewDir = normalize(cameraPos - ex_Position); // view direction from camera to fragment
	vec3 reflectDir = reflect(lightDir, norm); // reflect vector along the normal axis

	 // specular component, the higher the power, the higher the shininess value
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 35);
	vec3 specular = specularStrength * spec * lightColor;

	// Final Light
	out_Color =
	mix(texture(texture0, ex_Texture), texture(texture1, ex_Texture) * vec4(ex_Color, 1.), 0.4)
	* (vec4(ambient, 1.f) + vec4(diffuse, 1.f) + vec4(specular, 1.f));
}