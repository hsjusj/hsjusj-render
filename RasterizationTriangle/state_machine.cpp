#include "state_machine.h"

bool line_mode = false;

void PolygonMode(int mode)
{
	if (mode == HS_LINE)
	{
		line_mode = true;
	}
}