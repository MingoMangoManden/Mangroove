#pragma once

class vec2
{
	public:
		float x, y;

		vec2(float _x, float _y)
		{
			x = _x;
			y = _y;
		}

		void add(vec2 a)
		{
			x -= a.x;
			y -= a.y;
		}
	
		void subtract(vec2 a)
		{
			x -= a.x;
			y -= a.y;
		}
		
		// multiply
		// divide
};
