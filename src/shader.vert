#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 acolor;

uniform mat4 transform;

out vec3 v_color;

void main() {
	gl_Position = transform * vec4(pos, 1.0);
	v_color = acolor;
}
