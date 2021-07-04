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
void checkButtonClose(GLFWwindow* window);	//function for checking if the window should close when escape is pushed											//

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
		 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f 
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	//define how to interpret vertex data for location in vertex attribute with position 0
	glEnableVertexAttribArray(0);	//enable vertex attribute with position 0

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));	//define how to interpret vertex data for color in vertex attribute with position 1
	glEnableVertexAttribArray(1);	//enable vertex attribute with position 1

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	//define how to interpret vertex data for color in vertex attribute with position 2
	glEnableVertexAttribArray(2);	//enable vertex attribute with position 1
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
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);	//define glm vector
	glm::mat4 transMat = glm::mat4(1.0f);	//create identity matrix
	transMat = glm::rotate(transMat, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));	//add rotation to transform matrix
	transMat = glm::scale(transMat, glm::vec3(0.5f, 0.5f, 0.5f));	//add scale to transform matrix

	unsigned int transformLoc = glGetUniformLocation(baseShader.ID, "transMat");	//get location of transMat uniform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transMat));	//send matrix to uniform
	//==================================================================


	while (!glfwWindowShouldClose(window))	//renderloop which exits when the window is told to close
	{
		checkButtonClose(window);

		//rendering
		//==================================================================

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	//set clear color
		glClear(GL_COLOR_BUFFER_BIT);	//clear the color buffer with previosly set color

		//draw
		timeValue = glfwGetTime();
		offsetValue = (sin(timeValue) / 2.0f) + 0.5f;

		baseShader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void checkButtonClose(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)	//check if escape is being pressed
	{
		glfwSetWindowShouldClose(window, true);
	}
}
