
#version 410 core

out vec4 FragColor;

in vec3 v_color;

uniform float time;
uniform vec2 resolution;

void main() {
	FragColor = vec4(v_color, 1.0);
}