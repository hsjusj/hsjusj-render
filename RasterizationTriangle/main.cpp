#include <gl\glew.h>
#include <gl\freeglut.h>
#include <gl\wglew.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "math_3d.h"
#include "primitive.h"
#include "pipeline.h"
#include "state_machine.h"

#include "main.h"

int Point_Density = 10;

//透视投影信息
PersProjInfo p;
//渲染管线
Pipeline GamePipeline;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(240, 200);
	glutCreateWindow("Start");
	//gluOrtho2D(-WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2, WINDOW_HEIGHT / 2);

	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(KeyBoardCB);
	glutIdleFunc(RenderScene);

	glColor3f(1.0f, 1.0f, 0.0f);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error:%s\n", glewGetErrorString(res));
		return 0;
	}
	//开启垂直同步，限制帧率,0关闭
	//软渲染下好像没有用
	wglSwapIntervalEXT(1);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//设置为线框模式
	//PolygonMode(HS_LINE);

	glutMainLoop();
	return 0;
}

static void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;

	Matrix4f rotateMatrix, totalMatrix;
	rotateMatrix.InitRotateTransform(0.0f, Scale, 0.0f);

	Vertex v0(rotateMatrix*Vector3f(-0.5f, -0.5f, 0.0f), Vector3f(0.1f, 0.1f, 0.1f));
	Vertex v1(rotateMatrix*Vector3f(0.0f, 0.5f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f));
	Vertex v2(rotateMatrix*Vector3f(0.5f, -0.7f, 0.0f), Vector3f(0.1f, 0.1f, 0.1f));
	Triangle3D t0(v0, v1, v2);


	glBegin(GL_POINTS);

	Scale += 1.1f;
	
	t0.Show();

	glEnd();

	glutSwapBuffers();
}

void SetUpRC()
{
	SetPersProjInfo(p);
	GamePipeline.SetPerspectiveProj(p);
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

void KeyBoardCB(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'd':
		Point_Density += 1;
		printf("Point_Density : %d\n", Point_Density);
		break;
	case 'a':
		Point_Density -= 1;
		printf("Point_Density : %d\n", Point_Density);
		break;
	}
}

void SetPersProjInfo(PersProjInfo &p)
{
	p.FOV = 35.0f;
	p.Width = WINDOW_WIDTH;
	p.Height = WINDOW_HEIGHT;
	p.zNear = 0.1f;
	p.zFar = 100.0f;
}