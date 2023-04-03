#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "main.h"
#include "shader.h"

const char *TITLE = "Mangroove v0.0.4";
const int WIDTH = 800;
const int HEIGHT = 800;

float vertices[] = {
	
	// vertices          // colors
	1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f, // top right
    1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom left
    -1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f, // top left
	
	// ><
	// vertices          // colors
	/*-1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f, // top left
	-1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left
	0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f, // middle --
	1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f, // top right
	1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f // bottom right*/

};


unsigned int indices[] = {
	1, 2, 3,
	3, 0, 1

	// ><
	/*0, 1, 2,
	3, 2, 4*/
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

	// EBO, VBO & VAO
	
	// 
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	
	// use VAO for VBO & EBO
	glBindVertexArray(VAO);
	
	// vertices -> VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// indices -> EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// in vec3 pos in vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// in vec3 color in vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	shader shader = create_shader("src/shader.vert", "src/shader.frag");

	use(shader);
	set_float2(shader.program, "resolution", WIDTH, HEIGHT);
	
	char *title_fps = malloc(25);
	int frames = 0;
	float old_time = glfwGetTime();
	
	while (!glfwWindowShouldClose(window))
	{
		float new_time = glfwGetTime();
		float time_passed = new_time - old_time;

		frames++;

		if (time_passed > 1.0 || frames == 0)
		{
			float fps = frames / time_passed;
			
			sprintf(title_fps, "%s | %i FPS", TITLE, (int) fps);
			glfwSetWindowTitle(window, title_fps);

			frames = 0;
			old_time = new_time;
		}

		//update();


		process_input(window);

		glClearColor(0.13f, 0.13f, 0.13f, 1.0f); // skybox
		glClear(GL_COLOR_BUFFER_BIT);
		
		//glUseProgram(shader.program);
		use(shader);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		set_float2(shader.program, "cursor", (float) xpos, (float) ypos);
		
		// set uniforms
		float time = glfwGetTime();
		//int loc2 = glGetUniformLocation(shader.program, "time");
		//glUniform1f(loc2, time);
		set_float(shader.program, "time", time);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / 3);
		glDrawElements(GL_TRIANGLES, sizeof(vertices) / 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		glUseProgram(0);
	}
	free(title_fps);
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

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		close_window(window);
	}
	
	static bool lock = false, wf_mode = false;
	if (!glfwGetKey(window, GLFW_KEY_W))
	{
		lock = false;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && lock == false)
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
