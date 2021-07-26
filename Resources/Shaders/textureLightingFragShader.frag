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

struct SpotLight {
	vec3 pos;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float inCutoff;
	float outCutoff;

	//values for light falloff
	float constant;
	float linear;
	float quadratic;
};

//prototypes
vec3 dirLightCalc(DirLight light, vec3 normal, vec3 viewDir);
vec3 pointLightCalc(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 spotLightCalc(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//in
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

//out
out vec4 FragColor;

//uniforms
//==============================================
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[POINT_LIGHT_CNT];
uniform SpotLight spotLight;

uniform vec3 lightPos;
uniform vec3 viewPos;
//==============================================

//vec3
vec3 norm;
vec3 lightDir;	
vec3 endLight;
vec3 viewDir;

void main()
{
	norm = normalize(Normal);
	viewDir = normalize(viewPos - FragPos);
	
	//directional
	endLight = dirLightCalc(dirLight, norm, viewDir);

	//point
	for(int i = 0; i < POINT_LIGHT_CNT; i++)
	{
		endLight += pointLightCalc(pointLights[i], norm, FragPos, viewDir);
	}
	endLight += spotLightCalc(spotLight, norm, FragPos, viewDir);

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

vec3 pointLightCalc(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir;
	vec3 reflecDir;
	vec3 ambientLight;
	vec3 diffuseLight;
	vec3 specularLight;
	float diff;
	float spec;
	float dist;
	float attenuation;

	lightDir = normalize(light.pos - fragPos);

	//diffuse
	diff = max(dot(normal, lightDir), 0.0);

	//specular
	reflecDir = reflect(-lightDir, normal);
	spec = pow(max(dot(viewDir, reflecDir), 0.0), material.shininess);

	//attenuation
	dist = length(light.pos - fragPos);
	attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	//combine
	ambientLight = light.ambient * vec3(texture(material.diffuse, texCoord));
	diffuseLight = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	specularLight = light.specular * spec * vec3(texture(material.specular, texCoord));

	ambientLight *= attenuation;
	diffuseLight *= attenuation;
	specularLight *= attenuation;

	return (ambientLight + diffuseLight + specularLight);
}

vec3 spotLightCalc(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir;
	vec3 reflecDir;
	vec3 ambientLight;
	vec3 diffuseLight;
	vec3 specularLight;
	float diff;
	float spec;
	float dist;
	float attenuation;
	float theta;
	float epsilon;
	float lightIntensity;

	lightDir = normalize(light.pos - fragPos);

	theta = dot(lightDir, normalize(-light.direction));
	epsilon = light.inCutoff - light.outCutoff;
	lightIntensity = clamp((theta - light.outCutoff) / epsilon, 0.0, 1.0);

	//diffuse
	diff = max(dot(normal, lightDir), 0.0);

	//specular
	reflecDir = reflect(-lightDir, normal);
	spec = pow(max(dot(viewDir, reflecDir), 0.0), material.shininess);

	//attenuation
	dist = length(light.pos - fragPos);
	attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	//combine
	ambientLight = light.ambient * vec3(texture(material.diffuse, texCoord));
	diffuseLight = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	specularLight = light.specular * spec * vec3(texture(material.specular, texCoord));

	ambientLight *= attenuation * lightIntensity;
	diffuseLight *= attenuation * lightIntensity;
	specularLight *= attenuation * lightIntensity;

	return (ambientLight + diffuseLight + specularLight);
}