#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//#include <iostream>
//#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "main.h"

const char *TITLE = "Mangroove v0.0.4";
const int WIDTH = 800;
const int HEIGHT = 800;

const char *vertex_shader_source =
	"#version 410 core\n"
	"layout (location = 0) in vec3 pos;\n"
	"void main() {\n"
		"gl_Position = vec4(pos, 1.0);\n"
	"}\n";

const char *fragment_shader_source =
	"#version 410 core\n"
	"out vec4 FragColor;\n"
	"in vec4 gl_FragCoord;\n"
	"uniform float time;\n"
	"uniform vec2 resolution;\n"
	"void main() {\n"
		//"FragColor = vec4(0.18f, 0.576f, 0.235f, 1.0f);\n"
		//"FragColor = vec4(sin(time), sin(time*-1), 0.0f, 1.0f);\n"
		
		"vec2 st = gl_FragCoord.xy/resolution;\n"
		//"float distance = distance(st, vec2(sin(time), sin(time * -1)));\n"

		"vec3 color = vec3(st.x, st.y, sin(time));\n"
		"FragColor = vec4(color, 1.0);\n"
	"}\n";

float vertices[] = {

	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,

	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f
	
	// []
	/*-0.2f, -0.2f, 0.0f,
	0.2f, -0.2f, 0.0f,
	-0.2f, 0.2f, 0.0f,

	0.2f, -0.2f, 0.0f,
	0.2f, 0.2f, 0.0f,
	-0.2f, 0.2f, 0.0f*/
	
	// ><
	/*-1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f*/
};

int main()
{
	glfwInit();

	GLFWwindow* window = create_window();
	
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		printf("[ ERROR ]: Failed to initialize GLAD.\n");
		glfwTerminate();
		return 1;
	}

	printf("Welcome to Mangroove!\n");
	printf("[ DEBUG ]: Renderer: %s\n", glGetString(GL_RENDERER));
	
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // update viewport when window is resized

	// VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	
	// VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	
	// Shader stuff
	unsigned int vertex_shader;
	vertex_shader = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
	
	unsigned int fragment_shader;
	fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
	

	unsigned int shader_program;
	shader_program = create_program(vertex_shader, fragment_shader);
	
	// static uniforms
	glUseProgram(shader_program);
	int loc = glGetUniformLocation(shader_program, "resolution");
	glUniform2f(loc, WIDTH, HEIGHT);
	glUseProgram(0);


	while (!glfwWindowShouldClose(window))
	{
		process_input(window);
		
		//update();

		glClearColor(0.13f, 0.13f, 0.13f, 1.0f); // skybox
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shader_program);
		
		// set uniforms
		float time = glfwGetTime();
		int loc2 = glGetUniformLocation(shader_program, "time");
		glUniform1f(loc2, time);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / 3);
		//glDrawElements(GL_TRIANGLES, sizeof(vertices) / 3, GL_UNSIGNED_INT, NULL);

		glfwSwapBuffers(window);
		glfwPollEvents();

		glUseProgram(0);
	}

	glfwTerminate();
	return 0;
}

GLFWwindow* create_window()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "%s\n", "[ ERROR ]: Failed to create GLFW window.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	return window;
}

int compile_shader(const char *shader_source_code, unsigned int shader_type)
{
	unsigned int shader;
	shader = glCreateShader(shader_type);

	glShaderSource(shader, 1, &shader_source_code, NULL);
	glCompileShader(shader);

	// Checking for compile errors in fragment shader
	int success;
	char info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		printf("[ ERROR ]: Error while compiling shader: \n%s\n", info_log);
		//std::cout << "[ ERROR ]: Error while compiling fragment shader:\n" << info_log << std::endl;
	}

	return shader;
}

int create_program(int vertex_shader, int fragment_shader)
{
	unsigned int shader_program = glCreateProgram();

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	// Checking for program errors
	int success;
	char info_log[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

	if (!success)
	{
		printf("[ ERROR ]: Error while creating shader program: \n%s\n", info_log);
		//std::cout << "[ ERROR ]: Error while linking shaders to program:\n" << info_log3 << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program;
}

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		close_window(window);
	}
	
	static bool lock = false, wf_mode = false;
	if (!glfwGetKey(window, GLFW_KEY_W))
	{
		lock = false;
	}

	if (glfwGetKey(window, GLFW_KEY_W) && lock == false)
	{
		glPolygonMode(GL_FRONT_AND_BACK, (wf_mode) ? GL_LINE : GL_FILL);
		wf_mode = !wf_mode;
		lock = true;
	}

}


void close_window(GLFWwindow *window)
{
	glfwSetWindowShouldClose(window, 1);
}


void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) // update viewport
{
	glViewport(0, 0, width, height);
}
