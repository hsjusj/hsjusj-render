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
#include "camera.h"
#include "renderlist.h"

#include "main.h"

int Point_Density = 10;

float rotate_scale = 0.1f;
float Intensity = 1.0f;

//����
Vector3f Eye(0.0f, 0.0f, 1.0f);
//͸��ͶӰ��Ϣ
PersProjInfo p;
//��Ⱦ����
Pipeline GamePipeline;
//����׶
Vertex m_vertex[5];
Triangle3D triangles[4];
//��������
int m_index[12];
//���
Camera GameCamera;

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
	//����Ⱦ�º������ʹˢ��Ƶ�ʽ���
	wglSwapIntervalEXT(1);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	SetUpRC();
	glutMainLoop();
	return 0;
}

static void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	RenderList render_list;

	GamePipeline.WorldPos(0.0f, 0.0f, 3.0f);
	GamePipeline.Rotate(Vector3f(0.0f, rotate_scale, 0.0f));

	Eye = GameCamera.GetTarget();
	GamePipeline.SetCamera(GameCamera);

	m_vertex[0] = { GamePipeline.GetWVTrans() * Vector3f(0.0f, 1.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f) };
	m_vertex[1] = { GamePipeline.GetWVTrans() * Vector3f(-0.5f, -0.2f, 0.5f), Vector3f(1.0f, 0.0f, 0.0f) };
	m_vertex[2] = { GamePipeline.GetWVTrans() * Vector3f(-0.5f, -0.2f, -0.5f), Vector3f(0.0f, 1.0f, 0.0f) };
	m_vertex[3] = { GamePipeline.GetWVTrans() * Vector3f(0.5f, -0.2f, -0.5f), Vector3f(0.0f, 0.0f, 1.0f) };
	m_vertex[4] = { GamePipeline.GetWVTrans() * Vector3f(0.5f, -0.2f, 0.5f), Vector3f(0.2f, 0.2f, 0.2f) };

	Vertex floor_vertex0(GamePipeline.GetViewTrans() * Vector3f(-1.0f, -0.2f, 10.0f), Vector3f(1.0f, 1.0f, 1.0f));
	Vertex floor_vertex1(GamePipeline.GetViewTrans() * Vector3f(-1.0f, -0.2f, 0.0f), Vector3f(0.2f, 0.2f, 0.2f));
	Vertex floor_vertex2(GamePipeline.GetViewTrans() * Vector3f(1.0f, -0.2f, 0.0f), Vector3f(0.2f, 0.2f, 0.2f));
	Vertex floor_vertex3(GamePipeline.GetViewTrans() * Vector3f(1.0f, -0.2f, 10.0f), Vector3f(0.5f, 0.5f, 0.5f));
	Triangle3D floor(floor_vertex2, floor_vertex1, floor_vertex0);
	Triangle3D floor2(floor_vertex3, floor_vertex0, floor_vertex2);

	glBegin(GL_POINTS);
	render_list.AddTriangle(floor);
	render_list.AddTriangle(floor2);

	for (unsigned int i = 0, j = 0; i < 4; i++, j += 3)
	{
		triangles[i] = Triangle3D(m_vertex[m_index[j]], m_vertex[m_index[j + 1]], m_vertex[m_index[j + 2]]);
		render_list.AddTriangle(triangles[i]);
	}


	render_list.Render();

	glEnd();

	glutSwapBuffers();
}

static void SetUpRC()
{

	printf("F1 �˳�\n");
	printf("F2 �任ģʽ\n");
	printf("ad��ת  ws���ƻ��������ǿ��   �������Ҽ��ƶ������\n");
	printf("\n�˴θ�������:\n");
	printf("1.�Բ�����Ұ�ڵ���������޳�����ʵ���˽�ƽ��ü�\n");
	printf("\n�ڴ˸�л�����鼮�����İ���\n");
	printf("--��c++ Primer��\n");
	printf("--�����Դ�������Ӧ�á�\n");
	printf("--��3D��ѧ������ͼ������Ϸ������\n");
	printf("--����Ϸ����ܹ���\n");
	printf("--��Windows��Ϸ��̴�ʦ���ɡ�\n");
	printf("--��3D��Ϸ��̴�ʦ���ɡ�\n");


	SetPersProjInfo(p);
	GamePipeline.SetPerspectiveProj(p);
	GamePipeline.SetCamera(GameCamera);

	m_index[0] = 0; m_index[1] = 1; m_index[2] = 2;
	m_index[3] = 0; m_index[4] = 2; m_index[5] = 3;
	m_index[6] = 0; m_index[7] = 3; m_index[8] = 4;
	m_index[9] = 0; m_index[10] = 4; m_index[11] = 1;
}

void SpecialKeyBoardCB(int key, int x, int y)
{
	GameCamera.OnKeyboard(key);
	switch (key)
	{
	case GLUT_KEY_F2:
		if (shader_mode & RENDER_MODE_FACE)
		{
			PolygonMode(RENDER_MODE_LINE);
		}
		else if (shader_mode & RENDER_MODE_LINE)
		{
			PolygonMode(RENDER_MODE_NO_FACE);
		}
		else if (shader_mode & RENDER_MODE_NO_FACE)
		{
			PolygonMode(RENDER_MODE_FACE);
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
	case 'w':
		Intensity += 0.1f;
		break;
	case 's':
		Intensity -= 0.1f;
		break;
	}
}

static void InitializerGlutCallBacks()
{
	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	//�����¼�
	glutKeyboardFunc(NormalKeyBoardCB);
	glutSpecialFunc(SpecialKeyBoardCB);
}

void SetPersProjInfo(PersProjInfo &p)
{
	p.FOV = 35.0f;
	p.Width = WINDOW_WIDTH;
	p.Height = WINDOW_HEIGHT;
	p.zNear = ZNEAR;
	p.zFar = ZFAR;
}