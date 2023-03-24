#include <stdio.h>
#include <math.h>
#include "vec2.h"


int is_point_in_circle(float radius, vec2 circle_center, vec2 point);
int is_point_in_square(vec2 points[4], vec2 point);

int main()
{
	printf("Welcome to Mangroove\n");
	
	printf("%i\n", is_point_in_circle(8.5, vec2(5.0, 5.0), vec2(-3, 5)));
	
	/*vec2 point = vec2(5.0, 5.0);
	
	printf("X: %f Y: %f\n", point.x, point.y);

	point.subtract(vec2(4.0, 4.0));
	
	printf("X: %f Y: %f\n", point.x, point.y);*/
	
	return 0;
}

int is_point_in_circle(float radius, vec2 circle_center, vec2 point)
{
	float dx = fabsf(point.x - circle_center.x);
	float dy = fabsf(point.y - circle_center.y);

	float distance = sqrt( (dx*dx) + (dy*2) );
	
	if (distance <= radius)
	{
		return 0;
	}
	return 1;
}

int is_point_in_square(vec2 points[4], vec2 point)
{
	return 0;
}


