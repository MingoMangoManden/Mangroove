#include <stdio.h>
#include <cglm/cglm.h>

#include "camera.h"


Camera create_camera()
{
	Camera cam;

	mat4 transform;
	glm_mat4_identity(transform);
	glm_mat4_copy(transform, cam.transform);

	vec3 scale = { 0.5f, 0.5f, 0.5f };
	glm_vec3_copy(scale, cam.scale);

	return cam;
}

void rotate_camera(Camera *cam, float *direction, float degrees)
{
	glm_rotate(cam->transform, glm_rad(degrees), direction);
	// glm_rotate(transform, glm_rad(degrees), GLM_ZUP);
}

void scale_camera(Camera *cam)
{
	glm_scale(cam->transform, cam->scale);
}
