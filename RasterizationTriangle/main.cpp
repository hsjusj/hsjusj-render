#include <gl\glew.h>
#include <gl\freeglut.h>
#include <gl\wglew.h>

#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <cmath>

#include "math_3d.h"
#include "primitive.h"
#include "pipeline.h"
#include "state_machine.h"

#include "main.h"

int Point_Density = 10;

float rotate_scale = 0.0f;

//����
Vector3f Eye(0.0f, 0.0f, 1.0f);
//͸��ͶӰ��Ϣ
PersProjInfo p;
//��Ⱦ����
Pipeline GamePipeline;
//����׶
Vertex m_vertex[5];
Triangle3D t_list[4];
//����
int m_index[12];

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(240, 200);
	glutCreateWindow("Start");
	//gluOrtho2D(-WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2, WINDOW_HEIGHT / 2);

	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(NormalKeyBoardCB);
	glutSpecialFunc(SpecialKeyBoardCB);
	glutIdleFunc(RenderScene);

	glColor3f(1.0f, 1.0f, 0.0f);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error:%s\n", glewGetErrorString(res));
		return 0;
	}
	//������ֱͬ��������֡��,0�ر�
	//����Ⱦ�º���û����
	wglSwapIntervalEXT(1);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//����Ϊ�߿�ģʽ
	//PolygonMode(HS_LINE);
	SetUpRC();
	glutMainLoop();
	return 0;
}

static void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	GamePipeline.WorldPos(0.0f, 0.0f, 3.0f);
	GamePipeline.Rotate(Vector3f(0.0f, rotate_scale, 0.0f));

	m_vertex[0] = { GamePipeline.GetWPTrans() * Vector3f(0.0f, 0.7f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f) };
	m_vertex[1] = { GamePipeline.GetWPTrans() * Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(1.0f, 0.0f, 0.0f) };
	m_vertex[2] = { GamePipeline.GetWPTrans() * Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0.0f, 1.0f, 0.0f) };
	m_vertex[3] = { GamePipeline.GetWPTrans() * Vector3f(0.5f, -0.5f, -0.5f), Vector3f(0.0f, 0.0f, 1.0f) };
	m_vertex[4] = { GamePipeline.GetWPTrans() * Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0.2f, 0.2f, 0.2f) };

	for (unsigned int i = 0, j = 0; i < 4; i++, j += 3)
	{
		t_list[i] = Triangle3D(m_vertex[m_index[j]], m_vertex[m_index[j + 1]], m_vertex[m_index[j + 2]]);
	}

	glBegin(GL_POINTS);
	
	for (unsigned int i = 0; i < 4; i++)
	{
		t_list[i].Show();
	}

	glEnd();

	glutSwapBuffers();
}

void SetUpRC()
{

	printf("F1 �˳�\n");
	printf("F2 �任ģʽ\n");
	printf("ad��ת\n");
	printf("\n�ڴ˸�л�����鼮�����İ���\n");
	printf("--��c++ Primer��\n");
	printf("--�����Դ�������Ӧ�á�\n");
	printf("--��3D��ѧ������ͼ������Ϸ������\n");
	printf("--����Ϸ����ܹ���\n");
	printf("--��Windows��Ϸ��̴�ʦ���ɡ�\n");
	printf("--��3D��Ϸ��̴�ʦ���ɡ�\n");


	SetPersProjInfo(p);
	GamePipeline.SetPerspectiveProj(p);

	m_vertex[0] = { Vector3f(0.0f, 0.7f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f) };
	m_vertex[1] = { Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(0.2f, 0.2f, 0.2f) };
	m_vertex[2] = { Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0.2f, 0.2f, 0.2f) };
	m_vertex[3] = { Vector3f(0.5f, -0.5f, -0.5f), Vector3f(0.2f, 0.2f, 0.2f) };
	m_vertex[4] = { Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0.2f, 0.2f, 0.2f) };

	m_index[0] = 0; m_index[1] = 1; m_index[2] = 2;
	m_index[3] = 0; m_index[4] = 2; m_index[5] = 3;
	m_index[6] = 0; m_index[7] = 3; m_index[8] = 4;
	m_index[9] = 0; m_index[10] = 4; m_index[11] = 1;


	//for (unsigned int i = 0, j = 0; i < 4; i++, j += 3)
	//{
	//	t_list[i] = Triangle3D(m_vertex[m_index[j]], m_vertex[m_index[j + 1]], m_vertex[m_index[j + 2]]);
	//}
}

void DDA_2d(const Vector2f &v0, const Vector2f &v1)
{
	float dx = v1.x - v0.x;
	float dy = v1.y - v0.y;
	float dm = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
#ifndef CHANGE_DENSITY
	dx /= (dm * POINT_DENSITY);
	dy /= (dm * POINT_DENSITY);
	float x = v0.x, y = v0.y;
	for (int i = 0; i < dm * POINT_DENSITY; i++)
	{
		glColor3f(1.0f - x, y + 0.5f, 0.5f);
		glVertex2f(x, y);
		x += dx;
		y += dy;
	}
#else
	dx /= (dm * Point_Density);
	dy /= (dm * Point_Density);
	float x = v0.x, y = v0.y;
	for (int i = 0; i < dm * Point_Density; i++)
	{
		glColor3f(1.0f - x, y + 0.1f, 0.8f);
		glVertex2f(x, y);
		x += dx;
		y += dy;
	}
#endif
}

void DDA_3d(const Vector3f &v0, const Vector3f &v1)
{
	
	float dx = v1.x - v0.x;
	float dy = v1.y - v0.y;
	float dz = v1.z - v0.z;
	float dm = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	dm = dm > abs(dz) ? dm : abs(dz);
	
	dx /= dm * POINT_DENSITY;
	dy /= dm * POINT_DENSITY;
	dz /= dm * POINT_DENSITY;

	float x = v0.x, y = v0.y, z = v0.z;
	float distance = dm * POINT_DENSITY;
	for (int i = 0; i < distance; i++)
	{
		glVertex3f(x, y, z);
		x += dx;
		y += dy;
		z += dz;
	}
}

//glutSpecialFunc(SpecialKeyboardCB);

void SpecialKeyBoardCB(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F2:
		if (shader_mode & HS_FACE)
		{
			PolygonMode(HS_LINE);
		}
		else if (shader_mode & HS_LINE)
		{
			PolygonMode(HS_NO_FACE);
		}
		else if (shader_mode & HS_NO_FACE)
		{
			PolygonMode(HS_FACE);
		}
		break;
	case GLUT_KEY_F1:
		exit(0);
		break;
	}
}

void NormalKeyBoardCB(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'd':
		rotate_scale += 1.0f;
		break;
	case 'a':
		rotate_scale -= 1.0f;
		break;
	}
}

void SetPersProjInfo(PersProjInfo &p)
{
	p.FOV = 35.0f;
	p.Width = WINDOW_WIDTH;
	p.Height = WINDOW_HEIGHT;
	p.zNear = 0.5f;
	p.zFar = 100.0f;
}