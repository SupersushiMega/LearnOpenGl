#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ambientStrength = 0.1;
float specStrength = 0.5;

vec3 norm = normalize(Normal);

vec3 lightDir;	
vec3 endLight;
vec3 viewDir;
vec3 reflecDir;

//end results
vec3 ambiLight;
vec3 diffLight;
vec3 specLight;

float diff;
float spec;


void main()
{
	ambiLight =  light.ambient * material.ambient;
	
	lightDir = normalize(lightPos - FragPos);
	diff = max(dot(norm, lightDir), 0.0);	//calculate difference
	diffLight = light.diffuse * (diff * material.diffuse);

	viewDir = normalize(viewPos - FragPos);
	reflecDir = reflect(-lightDir, norm);
	spec = pow(max(dot(viewDir, reflecDir), 0.0), material.shininess);

	specLight =  light.specular * (spec * material.specular);

	endLight = ambiLight + diffLight + specLight;

	FragColor = vec4(endLight, 1.0);
}