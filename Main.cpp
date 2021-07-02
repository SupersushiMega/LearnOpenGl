#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);	//function used to change the viewport size in case of a resize from the user
void checkButtonClose(GLFWwindow* window);	//function for checking if the window should close when escape is pushed


int main()
{
	//initialisation of GLFW
	//==================================================================
	glfwInit();	//first initialisation

	//setup
	
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	//Version number bevor dot (x.)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);	//Version number after dot (.x)

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//Version number bevor dot (x.)
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

	glViewport(0, 0, 100, 800);	//define viewport

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	//set the framebuffer_size_callback function to be called whenever the window gets resized

	while (!glfwWindowShouldClose(window))	//renderloop which exits when the window is told to close
	{
		checkButtonClose(window);

		//rendering
		//==================================================================

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);	//set clear color
		glClear(GL_COLOR_BUFFER_BIT);	//clear the color buffer with previosly set color


		glfwSwapBuffers(window);	//swap back buffer(buffer thats being drawn on) and front buffer(buffer with image to be displayed)
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