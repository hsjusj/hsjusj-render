#ifndef RENDER_LIST_H__

#define RENDER_LIST_H__

#include <vector>

#include "math_3d.h"
#include "primitive.h"
#include "pipeline.h"

class RenderList 
{
public:
	RenderList();
	void Render();
	void AddTriangle(const Triangle3D&);
private:
	std::vector<Triangle3D> render_list;
};

#endif