#version 330 core

#define POINT_LIGHT_CNT 4 //define the amount of point lights

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//values for light falloff
	float constant;
	float linear;
	float quadratic;
};
//
//struct dirLight {
//	vec3 pos;
//	vec3 direction;
//
//	vec3 ambient;
//	vec3 diffuse;
//	vec3 specular;
//
//	float inCutoff;
//	float outCutoff;
//
//	//values for light falloff
//	float constant;
//	float linear;
//	float quadratic;
//};

vec3 dirLightCalc(DirLight light, vec3 normal, vec3 viewDir);

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ambientStrength = 0.1;
float specStrength = 0.5;

float dist;
float attenuation;

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
float epsilon;
float theta;
float lightIntensity;


void main()
{
	dist = length(light.pos - FragPos);
	attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));	//calculate light fallof

	ambiLight =  light.ambient * vec3(texture(material.diffuse, texCoord));
	
	lightDir = normalize(light.pos - FragPos);
	theta = dot(lightDir, normalize(-light.direction));
	epsilon = light.inCutoff - light.outCutoff;
	lightIntensity = clamp((theta - light.outCutoff) / epsilon, 0.0, 1.0);

	//lightDir = normalize(-light.direction);
	diff = max(dot(norm, lightDir), 0.0);	//calculate difference
	diffLight = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

	viewDir = normalize(viewPos - FragPos);
	reflecDir = reflect(-lightDir, norm);
	spec = pow(max(dot(viewDir, reflecDir), 0.0), material.shininess);

	specLight =  light.specular * spec * vec3(texture(material.specular, texCoord));

	//ambiLight *= attenuation;
	diffLight *= lightIntensity;
	specLight *= lightIntensity;

	endLight = ambiLight + diffLight + specLight; //+ vec3(texture(material.emission, texCoord));

	FragColor = vec4(endLight, 1.0);
}

vec3 dirLightCalc(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir;
	vec3 reflecDir;
	vec3 ambientLight;
	vec3 diffuseLight;
	vec3 specularLight;
	float diff;
	float spec;

	lightDir = normalize(-light.direction);

	//diffuse
	diff = max(dot(normal, lightDir), 0.0);

	//specular
	reflecDir = reflect(-lightDir, normal);
	spec = pow(max(dot(viewDir, reflecDir), 0.0), material.shininess);

	//combine
	ambientLight = light.ambient * vec3(texture(material.diffuse, texCoord));
	diffuseLight = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	specularLight = light.specular * spec * vec3(texture(material.specular, texCoord));
	return (ambientLight + diffuseLight + specularLight);
}