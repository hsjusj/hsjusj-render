#include "state_machine.h"

int shader_mode = RENDER_MODE_FACE;

void PolygonMode(int mode)
{
	if (mode & RENDER_MODE_FACE)
	{
		shader_mode = RENDER_MODE_FACE;
	}
	else if (mode & RENDER_MODE_LINE)
	{
		shader_mode = RENDER_MODE_LINE;
	}
	else if (mode & RENDER_MODE_NO_FACE)
	{
		shader_mode = RENDER_MODE_NO_FACE;
	}
}