#ifndef STATE_MACHINE_H__

#define STATE_MACHINE_H__

#define INVALID_VALUE_FLOAT 65535.0f


#define HS_FACE    0x0001   //表面着色
#define HS_LINE    0x0010   //线框模式
#define HS_NO_FACE 0x0100   //全部渲染 

#include <cstdio>

extern int shader_mode;

void PolygonMode(int mode);

#endif