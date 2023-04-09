#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "shader.h"


shader create_shader(char *vertex_shader, char *fragment_shader)
{
	char *v_source = get_file_contents(vertex_shader);
	char *f_source = get_file_contents(fragment_shader);
	
	unsigned int v_shader = compile_shader(v_source, GL_VERTEX_SHADER);
	unsigned int f_shader = compile_shader(f_source, GL_FRAGMENT_SHADER);

	unsigned int program = create_program(v_shader, f_shader);
	
	shader shader = { .program = program, .vertex_shader = v_shader, .fragment_shader = f_shader };

	return shader;
}

shader create_shader_from_sources(char *v_source, char *f_source)
{
	unsigned int v_shader = compile_shader(v_source, GL_VERTEX_SHADER);
	unsigned int f_shader = compile_shader(f_source, GL_FRAGMENT_SHADER);

	unsigned int program = create_program(v_shader, f_shader);
	
	shader shader = { .program = program, .vertex_shader = v_shader, .fragment_shader = f_shader };

	return shader;
}

char* get_file_contents(char *filename)
{
	//printf("Reading from file %s\n", filename);

	FILE *file;
	file = fopen(filename, "r");

	if (file == NULL)
	{
		printf("[ ERROR ]: Unable to read from file!\n");
		return "";
	}
	
	char *source = malloc(256*10); // IMPORTANT: allocate enough memory to store file contents - produces an error if exceeds limit.
	
	// setting the first element of the string to be \0 aka end character
	// https://stackoverflow.com/questions/1853619/strange-characters-appear-when-using-strcat-function-in-c
	source[0] = 0;

	char line_buffer[256];
	
	while(fgets(line_buffer, 256, file))
	{
		//printf("buffer: %s\n", line_buffer);
		strcat(source, line_buffer);
	}

	//printf("File contents: \n%s\n", source);

	fclose(file);

	return source;
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

	printf("Shader type: %s\n", str(shader_type));

	return shader;
}

void use(shader s)
{
	glUseProgram(s.program);
}

void set_bool(int shader_program, const char *name, bool value)
{
	int loc = glGetUniformLocation(shader_program, name);
	glUniform1i(loc, value);
}

void set_float(int shader_program, const char *name, float value)
{
	int loc = glGetUniformLocation(shader_program, name);
	glUniform1f(loc, value);
}

void set_float2(int shader_program, const char *name, float value1, float value2)
{
	int loc = glGetUniformLocation(shader_program, name);
	glUniform2f(loc, value1, value2);
}

void set_matrix4fv(int shader_program, const char *name, mat4 *matrix)
{
	int loc = glGetUniformLocation(shader_program, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, (float *) matrix);
}

void set_int(int shader_program, const char *name, int value)
{
	int loc = glGetUniformLocation(shader_program, name);
	glUniform1i(loc, value);
}
