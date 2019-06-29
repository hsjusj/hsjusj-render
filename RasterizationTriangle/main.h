#ifndef MAIN_H__

#define MAIN_H__

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define POINT_DENSITY 320  //dda画线密集程度

//#define CHANGE_DENSITY  //可变画线密集程度
extern int Point_Density;

void RenderScene();
void KeyBoardCB(unsigned char key, int x, int y);

void SetUpRC();
void SetPersProjInfo(PersProjInfo &p);

void DDA_2d(const Vector2f &v0, const Vector2f &v1);
void DDA_3d(const Vector3f &v0, const Vector3f &v1);

#endif