#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*
 * PROTOTYPES
 */

void render(int shader_program);
void close_window(GLFWwindow *window);
void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/*
 * GLOBAL VARIABLES
 */

//string TITLE = "Mangroove v0.0.2";
int WIDTH = 800;
int HEIGHT = 800;

int main()
{
	glfwInit();

	printf("[ INFO ]: Welcome to Mangroove!\n");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mangroove", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "%s\n", "[ ERROR ]: Failed to create GLFW window.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	
	// glad init
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		printf("[ ERROR ]: Failed to initialize GLAD.\n");
		glfwTerminate();
		return 1;
	}

	printf("[ INFO ]: Renderer: %s\n", glGetString(GL_RENDERER));
	
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // update viewport when window is resized

	// Vertices and VBO
	float vertices[] {
		// []
		-0.1f, -0.1f, 0.0f,
		0.1f, -0.1f, 0.0f,
		-0.1f, 0.1f, 0.0f,

		0.1f, -0.1f, 0.0f,
		0.1f, 0.1f, 0.0f,
		-0.1f, 0.1f, 0.0f
		
		// ><
		/*-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f*/
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//
	

	// Vertex Shader
	const char *vertex_shader_source = "#version 410 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"void main() {\n"
			"gl_Position = vec4(pos, 1.0);\n"
		"}\n";

	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	
	// Checking for compile errors in vertex shader
	int success;
	char info_log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		//printf("[ ERROR ]: Error while compiling shader:\n");
		std::cout << "[ ERROR ]: Error while compiling vertex shader:\n" << info_log << std::endl;
	}
	//
	//
	
	// Fragment Shader
	const char *fragment_shader_source = "#version 410 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
			"FragColor = vec4(0.18f, 0.576f, 0.235f, 1.0f);\n"
		"}\n";
	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	// Checking for compile errors in fragment shader
	int success2;
	char info_log2[512];
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success2);

	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log2);
		//printf("[ ERROR ]: Error while compiling shader:\n");
		std::cout << "[ ERROR ]: Error while compiling fragment shader:\n" << info_log2 << std::endl;
	}
	//
	//
	
	// Creating shader program
	unsigned int shader_program;
	shader_program = glCreateProgram();

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Checking for errors
	int success3;
	char info_log3[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success3);

	if (!success3)
	{
		std::cout << "[ ERROR ]: Error while linking shaders to program:\n" << info_log3 << std::endl;
	}
	//
	//
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);


	while (!glfwWindowShouldClose(window))
	{
		process_input(window);
		
		//update();
		//render(shader_program);
		glClearColor(0.13f, 0.13f, 0.13f, 1.0f); // skybox
		glClear(GL_COLOR_BUFFER_BIT);
		
		// draw
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / 3);
		//glDrawElements(GL_TRIANGLES, sizeof(vertices) / 3, GL_UNSIGNED_INT, NULL);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		close_window(window);
	}

	static bool lock=0, wf_mode=0;
	if(!glfwGetKey(window, GLFW_KEY_W)) {
		lock = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_W) && lock==0) {
		glPolygonMode(GL_FRONT_AND_BACK, (wf_mode=1-wf_mode)? GL_LINE : GL_FILL);
		lock = 1;
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
