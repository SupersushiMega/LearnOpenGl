#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"ShaderLoader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);	//function used to change the viewport size in case of a resize from the user
void checkButtonClose(GLFWwindow* window);	//function for checking if the window should close when escape is pushed
//
//const char* vertexShaderSource = "#version 330 core \n"		//vertex shader code
//"layout (location = 0) in vec3 aPos; \n"					//
//"layout (location = 1) in vec3 aColor;\n"					//
//"out vec3 vertexColor;\n"									//
//"void main() \n"											//
//"{ \n"														//
//"	gl_Position = vec4(aPos, 1.0); \n"						//
//"	vertexColor = aColor;\n"								//
//"} \0";														//
//
//
//const char* fragmentShaderSource = "#version 330 core \n"	//fragment shader code
//"out vec4 FragColor; \n"									//
//"in vec3 vertexColor;\n"									//
//"void main() \n"											//
//"{ \n"														//
//"	FragColor = vec4(vertexColor, 1.0); \n"					//
//"} \0";														//

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
	 	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f
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
	


	int success;	//variable to store if a check was succesfull
	char infoLog[512];	//variable for log Output

	////create vertex shader
	////==================================================================
	//unsigned int vertexShader;
	//vertexShader = glCreateShader(GL_VERTEX_SHADER);	//create vertex shader
	//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);	//attach vertex shader sourcecode
	//glCompileShader(vertexShader);	//compile vertex shader
	//
	//glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);	//get status of compilation
	//if (!success)	//check if the compilation was not successful
	//{
	//	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);	//get log info and store into infoLog
	//	std::cout << "ERROR: vertexshader failed compilation: info: " << infoLog << std::endl;	//output error message
	//}
	////==================================================================
	//
	////create fragment shader
	////==================================================================
	//unsigned int fragmentShader;
	//fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	//create fragment shader
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);	//attach fragment shader sourcecode
	//glCompileShader(fragmentShader);	//compile vertex shader
	//
	//glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);	//get status of compilation
	//if (!success)	//check if the compilation was not successful
	//{
	//	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);	//get log info and store into infoLog
	//	std::cout << "ERROR: fragmentshader failed compilation: info: " << infoLog << std::endl;	//output error message
	//}
	////==================================================================
	//
	////create shader programm
	////==================================================================
	//unsigned int shaderProgram;
	//shaderProgram = glCreateProgram();	//create shader programm object
	//glAttachShader(shaderProgram, vertexShader);	//attach vertex shader to shader program
	//glAttachShader(shaderProgram, fragmentShader);	//attach fragment shader to shader program
	//glLinkProgram(shaderProgram);	//link shaders to shader program
	//
	//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);	//get link status of programm
	//if (!success)	//check if the linking was not successful
	//{
	//	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);	//get log info and store into infoLog
	//	std::cout << "ERROR: shaderprogram failed linking: info: " << infoLog << std::endl;	//output error message
	//}
	//
	//glUseProgram(shaderProgram);	//activate shader programm
	//
	////delete shaders
	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);
	////==================================================================

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	//define how to interpret vertex data for location in vertex attribute with position 0
	glEnableVertexAttribArray(0);	//enable vertex attribute with position 0

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));	//define how to interpret vertex data for color in vertex attribute with position 1
	glEnableVertexAttribArray(1);	//enable vertex attribute with position 1

	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

	shader baseShader("D:/Coding/SushRay2D/Resources/Shaders/baseVertShader.vert", "D:/Coding/SushRay2D/Resources/Shaders/baseFragShader.frag");	//load shader


	while (!glfwWindowShouldClose(window))	//renderloop which exits when the window is told to close
	{
		checkButtonClose(window);

		//rendering
		//==================================================================

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);	//set clear color
		glClear(GL_COLOR_BUFFER_BIT);	//clear the color buffer with previosly set color

		//draw
		timeValue = glfwGetTime();
		greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		baseShader.use();
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
