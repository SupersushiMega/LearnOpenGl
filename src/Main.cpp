#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"ShaderLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);	//function used to change the viewport size in case of a resize from the user
void proccessInput(GLFWwindow* window);	//function for checking if the window should close when escape is pushed											//

//define camera
//==================================================================
glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);	//define position of camera
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);	//define front of camera
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);	//define up of camera
//==================================================================

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

	//define vertices
	float vertices[] = {
		 -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		- 0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
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

	unsigned int indices[] = {
		0, 1, 3,	//tri1 
		1, 2, 3		//tri2
	};

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);	//define how to interpret vertex data for location in vertex attribute with position 0
	glEnableVertexAttribArray(0);	//enable vertex attribute with position 0

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));	//define how to interpret vertex data for color in vertex attribute with position 1
	glEnableVertexAttribArray(1);	//enable vertex attribute with position 1
	//==================================================================

	float timeValue = glfwGetTime();
	float offsetValue = (sin(timeValue) / 2.0f) + 0.5f;

	shader baseShader("Resources/Shaders/baseVertShader.vert", "Resources/Shaders/baseFragShader.frag");	//load shader

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

	unsigned char* data = stbi_load("Resources/Textures/container.jpg", &width, &height, &nrChannels, 0);	//load image
	
	if (data)	//check if there is data is not NULL
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	//generate texture
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
	
	data = stbi_load("Resources/Textures/awesomeface.png", &width, &height, &nrChannels, 0);	//load image

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

	baseShader.use();
	baseShader.setInt("texSampler1", 0);	//set texture 1 to GL_TEXTURE0
	baseShader.setInt("texSampler2", 1);	//set texture 2 to GL_TEXTURE1

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

	unsigned int modelMatLoc = glGetUniformLocation(baseShader.ID, "modelMat");	//get location of modelMat uniform
	unsigned int viewMatLoc = glGetUniformLocation(baseShader.ID, "viewMat");	//get location of viewMat uniform
	unsigned int projMatLoc = glGetUniformLocation(baseShader.ID, "projMat");	//get location of projMat uniform
	//==================================================================

	glEnable(GL_DEPTH_TEST);	//enable depth testing

	const float r = 10.0f;
	float camX;
	float camZ;

	while (!glfwWindowShouldClose(window))	//renderloop which exits when the window is told to close
	{
		proccessInput(window);

		//rendering
		//==================================================================

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	//set clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear the color buffer with previosly set color

		//draw

		baseShader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		camX = sin(glfwGetTime()) * r;
		camZ = cos(glfwGetTime()) * r;

		viewMat = glm::lookAt(camPos, camPos + camFront, camUp);

		glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));	//send matrix to uniform
		glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));	//send matrix to uniform


		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			modelMat = glm::mat4(1.0f);
			modelMat = glm::translate(modelMat, cubePos[i]);
			modelMat = glm::rotate(modelMat, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
			baseShader.setMat4("modelMat", modelMat);	//send matrix to uniform
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
		camPos += camSpeed * camFront;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	//check if escape is being pressed
	{
		camPos -= camSpeed * camFront;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	//check if escape is being pressed
	{
		camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	//check if escape is being pressed
	{
		camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
	}

}
