#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>

#include "ShaderLoader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

struct Vertex 
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct Texture 
{
	unsigned int id;
	std::string type;
};

class Mesh 
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures);
	void Draw(shader& Shader);

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	void setupMesh();
};

class Model
{
public:
	Model(char* path)
	{
		loadModel(path);
	}
	void Draw(shader &Shader);
private:
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);	//function used to change the viewport size in case of a resize from the user
void mouse_callback(GLFWwindow* window, double xpos, double ypos);	//function to get mouse movement events
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);	//function for scrolling events

void proccessInput(GLFWwindow* window);	//function for checking if the window should close when escape is pushed

//define camera
//==================================================================
camera cam = camera();
//==================================================================

//variables used for delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//variables used for mouse movement
float lastX = 500.0f;
float lastY = 400.0f;
float yaw = 0.0f;
float pitch = 0.0f;
float fov = 45.0f;
bool firstMouse = true;

int main()
{
	//initialisation of GLFW
	//==================================================================
	glfwInit();	//first initialisation

	//setup
	
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	//Version number before dot (x.)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);	//Version number after dot (.x)

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//Version number before dot (x.)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	//Version number after dot (.x)

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//set to core profile

	//==================================================================

	//open Window
	//==================================================================
	GLFWwindow* window = glfwCreateWindow(1000, 800, "SushRay2D", NULL, NULL);	//create window object
	
	if (window == NULL)	//check if window has been created
	{
		std::cout << "ERROR: window could not be created" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	//make context of window the main context of current thread

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))	//initalize GLAD and check if the initialisation was successful
	{
		std::cout << "ERROR: GLAD could not be initialized" << std::endl;
		return -1;
	}

	glViewport(0, 0, 1000, 800);	//define viewport

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	//set the framebuffer_size_callback function to be called whenever the window gets resized
	glfwSetCursorPosCallback(window, mouse_callback);	//set mouse_callback to be called whenever the mouse moves
	glfwSetScrollCallback(window, scroll_callback);	//set scroll_callback to be called whenever the user scrolles

	//define vertices
	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glm::vec3 cubePos[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};


	unsigned int indices[] = {
		0, 1, 3,	//tri1 
		1, 2, 3		//tri2
	};

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	//create vertex buffer object
	unsigned int vertexBuff;
	glGenBuffers(1, &vertexBuff);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuff);	//bind vertexBuff to the GL_ARRAY_BUFFER target
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	//copy data from triVert into GL_ARRAY_BUFFER which is bound to vertexBuff

	//create elemental buffer object
	unsigned int elementBuff;
	glGenBuffers(1, &elementBuff);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuff);	//bind elementBuff to the GL_ELEMENT_ARRAY_BUFFER target
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	//copy data from triVert into GL_ELEMENT_ARRAY_BUFFER which is bound to vertexBuff

	//create vertex array object
	//==================================================================
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);	//generate vertex array object of size 1

	glBindVertexArray(VAO);	//bind vertex array

	//copy vertices from vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuff);	//bind vertex buff
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	//copy vertices from buffer

	//copy data from elemental buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuff);	//bind elementBuff to the GL_ELEMENT_ARRAY_BUFFER target
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	//copy data from triVert into GL_ELEMENT_ARRAY_BUFFER which is bound to vertexBuff

	//set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	//define how to interpret vertex data for location in vertex attribute with position 0
	glEnableVertexAttribArray(0);	//enable vertex attribute with position 0

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));	//define how to interpret vertex data for color in vertex attribute with position 1
	glEnableVertexAttribArray(1);	//enable vertex attribute with position 1

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	//define how to interpret vertex data for texture coordinates in vertex attribute with position 2
	glEnableVertexAttribArray(2);	//enable vertex attribute with position 2
	//==================================================================

	//create vertex array object for light source
	//==================================================================
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);	//generate vertex array object of size 1
	glBindVertexArray(lightVAO);	//bind vertex array
	
	//copy vertices from vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuff);	//bind vertex buff
	
	//copy data from elemental buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuff);	//bind elementBuff to the GL_ELEMENT_ARRAY_BUFFER target
	
	//set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	//define how to interpret vertex data for location in vertex attribute with position 0
	glEnableVertexAttribArray(0);	//enable vertex attribute with position 0
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));	//define how to interpret vertex data for color in vertex attribute with position 1
	glEnableVertexAttribArray(1);	//enable vertex attribute with position 1

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	//define how to interpret vertex data for color in vertex attribute with position 1
	glEnableVertexAttribArray(2);	//enable vertex attribute with position 1
	//==================================================================

	shader baseShader("Resources/Shaders/baseVertShader.vert", "Resources/Shaders/baseFragShader.frag");	//load shader

	shader textureShader("Resources/Shaders/textureVertShader.vert", "Resources/Shaders/textureFragShader.frag");	//load shader

	shader lightingTextureShader("Resources/Shaders/textureLightingVertShader.vert", "Resources/Shaders/textureLightingFragShader.frag");	//load shader
	lightingTextureShader.use();
	lightingTextureShader.setVec3("lightPos", lightPos);
	lightingTextureShader.setVec3("viewPos", cam.camPos);

	lightingTextureShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
	lightingTextureShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	lightingTextureShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingTextureShader.setFloat("material.shininess", 32.0f);

	lightingTextureShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	lightingTextureShader.setVec3("dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	lightingTextureShader.setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingTextureShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	lightingTextureShader.setVec3("pointLights[0].ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	lightingTextureShader.setVec3("pointLights[0].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingTextureShader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingTextureShader.setVec3("pointLights[0].pos", pointLightPositions[0]);
	lightingTextureShader.setFloat("pointLights[0].constant", 1.0f);
	lightingTextureShader.setFloat("pointLights[0].linear", 1.0f);
	lightingTextureShader.setFloat("pointLights[0].quadratic", 1.0f);

	lightingTextureShader.setVec3("pointLights[1].ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	lightingTextureShader.setVec3("pointLights[1].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingTextureShader.setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingTextureShader.setVec3("pointLights[1].pos", pointLightPositions[1]);
    lightingTextureShader.setFloat("pointLights[1].constant", 1.0f);
    lightingTextureShader.setFloat("pointLights[1].linear", 1.0f);
    lightingTextureShader.setFloat("pointLights[1].quadratic", 1.0f);

	lightingTextureShader.setVec3("pointLights[2].ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	lightingTextureShader.setVec3("pointLights[2].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingTextureShader.setVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingTextureShader.setVec3("pointLights[2].pos", pointLightPositions[2]);
    lightingTextureShader.setFloat("pointLights[2].constant", 1.0f);
    lightingTextureShader.setFloat("pointLights[2].linear", 1.0f);
    lightingTextureShader.setFloat("pointLights[2].quadratic", 1.0f);

	lightingTextureShader.setVec3("pointLights[3].ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	lightingTextureShader.setVec3("pointLights[3].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingTextureShader.setVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingTextureShader.setVec3("pointLights[3].pos", pointLightPositions[3]);
    lightingTextureShader.setFloat("pointLights[3].constant", 1.0f);
    lightingTextureShader.setFloat("pointLights[3].linear", 1.0f);
    lightingTextureShader.setFloat("pointLights[3].quadratic", 1.0f);

	lightingTextureShader.setFloat("spotLight.inCutoff", glm::cos(glm::radians(12.5f)));
	lightingTextureShader.setFloat("spotLight.outCutoff", glm::cos(glm::radians(17.5f)));
	lightingTextureShader.setFloat("spotLight.constant", 1.0f);
	lightingTextureShader.setFloat("spotLight.linear", 1.0f);
	lightingTextureShader.setFloat("spotLight.quadratic", 1.0f);
	lightingTextureShader.setVec3("spotLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	lightingTextureShader.setVec3("spotLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingTextureShader.setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	shader lightSourceShader("Resources/Shaders/baseVertShader.vert", "Resources/Shaders/lightSourceFragShader.frag");
	lightSourceShader.use();
	lightSourceShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	//define texture Parameters and load textures
	//==================================================================

	//texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	//set texture wrapping on the s axis to mirrored repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//set texture wrapping on the t axis to clamp to border

	float borderColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);	//set border color

	//texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	//set linear filtering mode mode for downscaling on texture and mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//set linear filtering mode for upscaling on texture

	//load textures
	int width;
	int height;
	int nrChannels;	//number of color channels
	
	unsigned int texture1;
	glGenTextures(1, &texture1);	//generate texture object
	glBindTexture(GL_TEXTURE_2D, texture1);	//bind texture to GL_TEXTURE_2D

	stbi_set_flip_vertically_on_load(true);	//set image loader to load image flipped on y axis

	unsigned char* data = stbi_load("Resources/Textures/container2.png", &width, &height, &nrChannels, 0);	//load image
	
	if (data)	//check if there is data is not NULL
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	//generate texture
		glGenerateMipmap(GL_TEXTURE_2D);	//generate Mipmap
	}
	else
	{
		std::cout << "ERROR: texture loading failed" << std::endl;
	}

	stbi_image_free(data);	//free image memory
	

	unsigned int texture2;
	glGenTextures(1, &texture2);	//generate texture object
	glBindTexture(GL_TEXTURE_2D, texture2);	//bind texture to GL_TEXTURE_2D
	
	data = stbi_load("Resources/Textures/container2_specular.png", &width, &height, &nrChannels, 0);	//load image

	if (data)	//check if there is data is not NULL
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	//generate texture
		glGenerateMipmap(GL_TEXTURE_2D);	//generate Mipmap
	}
	else
	{
		std::cout << "ERROR: texture loading failed" << std::endl;
	}

	stbi_image_free(data);	//free image memory
	
	unsigned int texture3;

	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	
	data = stbi_load("Resources/Textures/matrix.jpg", &width, &height, &nrChannels, 0);
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR: texture loading failed" << std::endl;
	}
	
	stbi_image_free(data);

	textureShader.use();
	textureShader.setInt("texSampler1", 0);	//set texture 1 to GL_TEXTURE0
	textureShader.setInt("texSampler2", 1);	//set texture 2 to GL_TEXTURE1

	float ratio = 0.0f;
	//==================================================================


	//define Matrixes
	//==================================================================
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::rotate(modelMat, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));	//create model matrix

	glm::mat4 viewMat;
	viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));	//create view matrix

	glm::mat4 projMat;
	projMat = glm::perspective(glm::radians(45.0f), 1000.0f / 800.0f, 0.1f, 100.0f);	//create projection matrix with perspective

	unsigned int modelMatLocObject = glGetUniformLocation(lightingTextureShader.ID, "modelMat");	//get location of modelMat uniform
	unsigned int viewMatLocObject= glGetUniformLocation(lightingTextureShader.ID, "viewMat");	//get location of viewMat uniform
	unsigned int projMatLocObject= glGetUniformLocation(lightingTextureShader.ID, "projMat");	//get location of projMat uniform

	unsigned int modelMatLocSource = glGetUniformLocation(lightSourceShader.ID, "modelMat");	//get location of modelMat uniform
	unsigned int viewMatLocSource= glGetUniformLocation(lightSourceShader.ID, "viewMat");	//get location of viewMat uniform
	unsigned int projMatLocSource= glGetUniformLocation(lightSourceShader.ID, "projMat");	//get location of projMat uniform
	//==================================================================

	glEnable(GL_DEPTH_TEST);	//enable depth testing

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	//capture mouse cursor

	cam.setMoveMode(FLY_MODE);

	glm::vec3 lightCol;
	glm::vec3 diffCol;
	glm::vec3 ambiCol;

	//assign textures
	//==================================================================
	lightingTextureShader.use();
	lightingTextureShader.setInt("material.diffuse", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	lightingTextureShader.setInt("material.specular", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//lightingTextureShader.setInt("material.emission", 2);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, texture3);
	//==================================================================

	float angle = 0;

	while (!glfwWindowShouldClose(window))	//renderloop which exits when the window is told to close
	{
		proccessInput(window);


		//rendering
		//==================================================================

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	//set clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear the color buffer with previosly set color

		//draw
		viewMat = cam.getViewMat();
		projMat = glm::perspective(glm::radians(cam.fov), 1000.0f / 800.0f, 0.1f, 100.0f);

		//draw cube
		lightingTextureShader.use();
		for (unsigned int i = 0; i < 10; i++)
		{
			angle = 20.0f * i;
			glUniformMatrix4fv(viewMatLocObject, 1, GL_FALSE, glm::value_ptr(viewMat));	//send matrix to uniform
			glUniformMatrix4fv(projMatLocObject, 1, GL_FALSE, glm::value_ptr(projMat));	//send matrix to uniform
			modelMat = glm::mat4(1.0f);
			modelMat = glm::translate(modelMat, cubePos[i]);
			modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingTextureShader.setMat4("modelMat", modelMat);	//send matrix to uniform
			lightingTextureShader.setVec3("viewPos", cam.camPos);
			lightingTextureShader.setVec3("spotLight.pos", cam.camPos);
			lightingTextureShader.setVec3("spotLight.direction", cam.camMoveFront);
			//lightingTextureShader.setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//draw lightsource
		for (unsigned int i = 0; i < 4; i++)
		{
			lightSourceShader.use();
			glUniformMatrix4fv(viewMatLocSource, 1, GL_FALSE, glm::value_ptr(viewMat));	//send matrix to uniform
			glUniformMatrix4fv(projMatLocSource, 1, GL_FALSE, glm::value_ptr(projMat));	//send matrix to uniform
			modelMat = glm::mat4(1.0f);
			modelMat = glm::translate(modelMat, pointLightPositions[i]);
			modelMat = glm::scale(modelMat, glm::vec3(0.2f, 0.2f, 0.2f));
			lightSourceShader.setMat4("modelMat", modelMat);	//send matrix to uniform
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);	//swap back buffer (buffer thats being drawn on) and front buffer(buffer with image to be displayed)
		//==================================================================

		glfwPollEvents();	//check for events, update window state and call coresponding functions
	}

	glfwTerminate();	//clear allocated resources
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);	//set viewport to new dimensions
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = 0;
	float yoffset = 0;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	else
	{
		xoffset = xpos - lastX;
		yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;
	}

	cam.mouseYawPitch(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
	{
		fov = 1.0f;
	}
	else if (fov > 90.0f)
	{
		fov = 90.0f;
	}
	cam.setFOV(fov);
}

void proccessInput(GLFWwindow* window)
{
	float curFrame = glfwGetTime();
	deltaTime = curFrame - lastFrame;	//caclulate delta time
	lastFrame = curFrame;
	const float camSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)	//check if escape is being pressed
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)	//check if escape is being pressed
	{
		cam.camMove(1.0f, 0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	//check if escape is being pressed
	{
		cam.camMove(-1.0f, 0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	//check if escape is being pressed
	{
		cam.camMove(0.0f, 1.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	//check if escape is being pressed
	{
		cam.camMove(0.0f, -1.0f, 0.0f);
	}
};


//mesh class
//========================================================================================================
Mesh::Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures)
{
	this->vertices = Vertices;
	this->indices = Indices;
	this->textures = Textures;

	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	glBindVertexArray(0);
}

void Mesh::Draw(shader& Shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	std::string number;
	std::string name;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		name = textures[i].type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}

		Shader.setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
//========================================================================================================

//Model class
//========================================================================================================
void Model::Draw(shader& Shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(Shader);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP ERROR" << importer.GetErrorString() << std::endl;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//process node meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	//pocess meshes of child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::vector<Texture> diffuseMaps;
	std::vector<Texture> specularMaps;
	glm::vec3 vector;
	glm::vec2 texVec;
	Vertex vertex;

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.pos = vector;

		if (mesh->mTextureCoords[0])
		{
			texVec.x = mesh->mTextureCoords[0][i].x;
			texVec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = texVec;
		}
		else
		{
			vertex.texCoord = glm::vec2(0.0f, 0.0f);
		}

	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
};

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	aiString str;
	Texture texture;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		mat->GetTexture(type, i, &str);
		texture.id = std::TextureFromFile(str.C_Str(), directory);
		texture.type = typeName;
		texture.path = str;
		textures.push_back(texture);
	}
}
//========================================================================================================