#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "main.h"
#include "shader.h"

#define TITLE "Mangroove v0.0.5"
#define WIDTH 800
#define HEIGHT 800

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

float rotation = 0.0f;
vec4 offset = { 0.0f, 0.0f, 0.0f, 1.0f };

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
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	
	// use VAO for VBO
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


	/* camera */



	/* end camera */
	
	char *title_fps = malloc(25);
	int frames = 0;
	float old_time = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		frames++;
		update_fps(window, old_time, frames, title_fps);
		
		// shader uniforms
		use(shader);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		set_float2(shader.program, "cursor", (float) xpos, (float) ypos);
		
		float time = glfwGetTime();
		set_float(shader.program, "time", time);
		
		// camera
		mat4 trans;
		glm_mat4_identity(trans);

		glm_rotate(trans, glm_rad(rotation), GLM_ZUP);
		/*glm_scale(trans, (vec3)
				{ sin(glfwGetTime()), sin(glfwGetTime()), sin(glfwGetTime()) }
		);*/
		vec3 scale = { 0.5f, 0.5f, 0.5f };
		glm_scale(trans, scale);

		//vec = trans * vec;
		glm_translate(trans, offset);
		//rotation += -1.0f;
		//printf("%f, %f, %f\n", vec[0], vec[1], vec[2]);

		set_matrix4fv(shader.program, "transform", &trans);


		process_input(window);

		glClearColor(0.13f, 0.13f, 0.13f, 1.0f); // skybox
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(vertices) / 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		glUseProgram(0);
	}

	free(title_fps);
	glfwTerminate();

	return 0;
}

void update_fps(GLFWwindow* window, float old_time, int frames, char *title_fps)
{
	float new_time = glfwGetTime();
	float time_passed = new_time - old_time;

	if (time_passed > 1.0 || frames == 0)
	{
		float fps = frames / time_passed;
		sprintf(title_fps, "%s | %i FPS", TITLE, (int) fps);
		glfwSetWindowTitle(window, title_fps);
		frames = 0;
		old_time = new_time;
	}
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
	if (!glfwGetKey(window, GLFW_KEY_E))
	{
		lock = false;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && lock == false)
	{
		glPolygonMode(GL_FRONT_AND_BACK, (wf_mode) ? GL_LINE : GL_FILL);
		wf_mode = !wf_mode;
		lock = true;
	}

	/* Controls */

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		offset[1] += 0.01f;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		offset[1] -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		offset[0] -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		offset[0] += 0.01f;

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		rotation += 10.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		rotation -= 1.0f;
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
