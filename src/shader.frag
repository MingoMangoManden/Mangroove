#version 410 core

out vec4 FragColor;

in vec4 gl_FragCoord;
in vec3 v_color;

uniform float time;
uniform vec2 resolution;
uniform vec2 cursor;

float random (vec2 st, vec2 m) {
    return fract(sin(st.x * st.y) * time);
}

void main() {
	vec2 st = gl_FragCoord.xy/resolution;
	vec2 m = cursor.xy/resolution;

	//float pct = abs(sin(time * (st.x + st.y) * 10));
	//FragColor = vec4(pct, 0.0, pct, 1.0);
	//float pct = abs(sin((st.x + st.y + time) * 10));
	//float pct = abs(sin(time * (st.x + st.y * time) * 10));
	//float pct = abs(sin(time * (st.x * st.y) * 10));
	//float pct = abs(sin((st.x * st.y + time) * 10));

	vec3 color = vec3(random(floor(st*100.0), m));

	FragColor = vec4(color, 1.0);
}

