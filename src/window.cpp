#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;


/*
 * PROTOTYPES
 */


void close_window(GLFWwindow *window);
void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/*
 * GLOBAL VARIABLES
 */

string TITLE = "Mangroove v0.0.2";
int WIDTH = 800;
int HEIGHT = 800;


int main()
{
	glfwInit();

	printf("Creating window!\n");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mangroove", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "%s\n", "ERROR: Failed to create GLFW window.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	
	// glad init
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD.\n");
		glfwTerminate();
		return 1;
	}
	
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // update viewport when window is resized

	while (!glfwWindowShouldClose(window))
	{
		process_input(window);
		
		//update();
		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void render()
{
	glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		close_window(window);
	}
}

void close_window(GLFWwindow *window)
{
	glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) // update viewport
{
	glViewport(0, 0, width, height);
}
