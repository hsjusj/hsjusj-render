#ifndef MAIN_H__

#define MAIN_H__

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define ZNEAR 0.5f
#define ZFAR 100.0f

#define POINT_DENSITY 320  //dda画线密集程度

//#define CHANGE_DENSITY  //可变画线密集程度
extern int Point_Density;
extern float Intensity;
extern Vector3f Eye;

class Pipeline;
extern Pipeline GamePipeline;

static void InitializerGlutCallBacks();
static void RenderScene();
void NormalKeyBoardCB(unsigned char key, int x, int y);
void SpecialKeyBoardCB(int key, int x, int y);

static void SetUpRC();
void SetPersProjInfo(PersProjInfo &p);

#endif