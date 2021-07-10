#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ambientStrength = 0.1;
float specStrength = 0.5;

vec3 norm = normalize(Normal);

vec3 ambientLight;
vec3 lightDir;	
vec3 diffuseLight;
vec3 endLight;
vec3 viewDir;
vec3 reflecDir;
vec3 specLight;

float diff;
float spec;


void main()
{
	ambientLight = ambientStrength * lightColor;
	lightDir = normalize(lightPos - FragPos);
	diff = max(dot(norm, lightDir), 0.0);	//calculate difference
	diffuseLight = diff * lightColor;

	viewDir = normalize(viewPos - FragPos);
	reflecDir = reflect(-lightDir, norm);
	spec = pow(max(dot(viewDir, reflecDir), 0.0), 32);
	specLight = specStrength * spec * lightColor;

	endLight = (ambientLight + diffuseLight + specLight) * objectColor;

	FragColor = vec4(endLight, 1.0);
}