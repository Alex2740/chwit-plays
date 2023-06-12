#include "api.h"


GLfloat Api::functionColorBackgound(float x, int stepShit)
{
	GLfloat step = 1.0f / 6.0f;
	GLfloat new_x = x - stepShit * step;

	if (new_x < 0) new_x += 1;
	if (new_x > 1) new_x -= 1;

	if (new_x <= step) return new_x / step;
	if (new_x <= 3 * step) return 1.0f;
	if (new_x <= 4 * step) return -new_x / step + 4.0f;
	return 0.0f;
}
