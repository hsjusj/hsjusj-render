#ifndef STATE_MACHINE_H__

#define STATE_MACHINE_H__

#define RENDER_MODE_FACE    0x0001   //�����޳�
#define RENDER_MODE_LINE    0x0010   //�߿�ģʽ
#define RENDER_MODE_NO_FACE 0x0100   //ȫ����Ⱦ

#include <cstdio>

extern int shader_mode;

void PolygonMode(int mode);

#endif