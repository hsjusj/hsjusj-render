#include "state_machine.h"

int shader_mode = HS_FACE;

void PolygonMode(int mode)
{
	if (mode & HS_FACE)
	{
		shader_mode = HS_FACE;
	}
	else if (mode & HS_LINE)
	{
		shader_mode = HS_LINE;
	}
	else if (mode & HS_NO_FACE)
	{
		shader_mode = HS_NO_FACE;
	}
}