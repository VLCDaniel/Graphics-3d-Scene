// Fragment Shader - Second
// GLSL (OpenGL Shading Language) 
#version 400

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

// Input Values (from Vertex Shader)
in vec3 ex_Position;
in vec3 ex_Color;
in vec2 ex_Texture;
in vec3 ex_Normal;

// Output values
out vec4 out_Color;

// Uniforms
uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPos;

// Local Variables
vec3 lightDir; // normalised direction from the light source to the normal
vec3 norm;	   // normal 

// Functions
vec3 calculateAmbient()
{
	return material.ambient;
}

vec3 calculateDiffuse()
{
	// normalised direction from the light source to the normal
	lightDir = normalize(lightPos0 - ex_Position);
	norm = normalize(ex_Normal); // make sure the normal is normalised

	float diff = max(dot(norm, lightDir), 0.0); // get angle between lightDir and norm
	return diff * material.diffuse; // return the diffuse component -> larger angle -> lower light
}

vec3 calculateSpecular()
{
	float specularStrength = 0.7; // intensity value
	vec3 viewDir = normalize(cameraPos - ex_Position); // view direction from camera to fragment
	vec3 reflectDir = reflect(-lightDir, norm); // reflect vector along the normal axis

	// specular component, the higher the power, the higher the shininess value
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 35);
	return (specularStrength * spec * material.specular * texture(material.specularTex, ex_Texture).rgb);
}

void main(void)
{
	// PHONE LIGHTING

	// Ambient light -> basic light
	vec3 ambient = calculateAmbient();

	// Diffuse light -> light from source
	vec3 diffuse = calculateDiffuse(); // get the diffuse component -> larger angle -> lower light

	// Specular light -> reflection
	vec3 specular = calculateSpecular();

	// Final Light
//	out_Color =
//	mix(texture(material.diffuseTex, ex_Texture), texture(material.specularTex, ex_Texture) * vec4(ex_Color, 1.), 0.4)
//	* (vec4(ambient, 1.f) + vec4(diffuse, 1.f) + vec4(specular, 1.f));

	out_Color =
	texture(material.diffuseTex, ex_Texture)
	* (vec4(ambient, 1.f) + vec4(diffuse, 1.f) + vec4(specular, 1.f));

//	out_Color = texture(material.diffuseTex, ex_Texture) * (vec4(ambient, 1.f) + vec4(diffuse, 1.f) + vec4(specular, 1.f));
}