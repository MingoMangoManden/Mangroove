
#version 410 core

out vec4 FragColor;

in vec4 gl_FragCoord;
in vec3 v_color;

uniform float time;
uniform vec2 resolution;
void main() {
	//FragColor = vec4(0.18f, 0.576f, 0.235f, 1.0f);
	
	//vec2 st = gl_FragCoord.xy/resolution;
	//vec3 color = vec3(st.x, st.y, sin(time));

	FragColor = vec4(v_color, 1.0);
}