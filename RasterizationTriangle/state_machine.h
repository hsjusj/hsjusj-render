#ifndef STATE_MACHINE_H__

#define STATE_MACHINE_H__

#define INVALID_VALUE_FLOAT 65535.0f


#define HS_FACE    0x0001   //������ɫ
#define HS_LINE    0x0010   //�߿�ģʽ
#define HS_NO_FACE 0x0100   //ȫ����Ⱦ 

#include <cstdio>

extern int shader_mode;

void PolygonMode(int mode);

#endif