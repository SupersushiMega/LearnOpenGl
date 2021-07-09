#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

float ambientStrength = 0.1;
vec3 ambientLight;

vec3 norm = normalize(Normal);
vec3 lightDir;	
float diff;
vec3 diffuseLight;

vec3 endLight;

void main()
{
	ambientLight = ambientStrength * lightColor;
	lightDir = normalize(lightPos - FragPos);
	diff = max(dot(norm, lightDir), 0.0);	//calculate difference
	diffuseLight = diff * lightColor;
	endLight = (ambientLight + diffuseLight) * objectColor;

	FragColor = vec4(endLight, 1.0);
}