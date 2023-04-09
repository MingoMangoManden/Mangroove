#pragma once

#include <cglm/cglm.h>

typedef struct
{
	unsigned int program;
	unsigned int vertex_shader, fragment_shader;
} shader;

shader create_shader(char *vertex_shader, char *fragment_shader);
shader create_shader_from_sources(char *v_source, char *f_source);

char* get_file_contents(char *filename);
void use(shader s);

int create_program(int vertex_shader, int fragment_shader);
int compile_shader(const char *shader_source_code, unsigned int shader_type);

void set_bool(int shader_program, const char *name, bool value);
void set_float(int shader_program, const char *name, float value);
void set_float2(int shader_program, const char *name, float value1, float value2);
void set_matrix4fv(int shader_program, const char *name, mat4 *matrix);
void set_int(int shader_program, const char *name, int value);
