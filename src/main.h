#pragma once

int compile_shader(const char* shader_source_code, unsigned int shader_type);
void process_input(GLFWwindow* window);
void close_window(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, const int width, const int height);
GLFWwindow* create_window();
int create_program(int vertex_shader, int fragment_shader);
