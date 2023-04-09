#pragma once

#include <cglm/cglm.h>

typedef struct
{
	mat4 transform;
	vec3 scale;
} Camera;

Camera create_camera();
void rotate_camera(Camera *cam, float *direction, float degrees);
void scale_camera(Camera *cam);
