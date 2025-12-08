#include <graphics.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <string>
#include <sstream>


typedef enum
{
	NORMAL,
	SLEEP,
	CLICK,
	LONGPRESS,
	DRAG,
	KEYBOARD
}PetState;

typedef struct
{
	int x, y;
	int height, width;
	PetState state;
	int stateTimer;
}Pet;

typedef struct
{
	int height, width;
	HWND hwnd;
}Window;

Pet pet;
Window win;
IMAGE imgNormal;
IMAGE imgSleep;
IMAGE imgClick;
IMAGE imgDrag;
IMAGE imgLongpress;
IMAGE imgKeyboard;

void InitPet()
{
	pet.x = 400;
	pet.y = 300;
	pet.width = 200;
	pet.height = 200;
	pet.state = NORMAL;
	pet.stateTimer = 0;
}

void InitWindow()
{
	win.height = 300;
	win.width = 400;
	initgraph(400, 300,EX_NOCLOSE|EX_NOMINIMIZE);
	win.hwnd = GetHWnd();
	//设置无边框
	SetWindowLongPtrA(win.hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	//设置窗口透明
	//1.设置分层窗口
	SetWindowLongPtrA(win.hwnd, GWL_EXSTYLE,
		GetWindowLongPtrA(win.hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	//2.设置透明色，填充透明色
	SetLayeredWindowAttributes(win.hwnd, RGB(1,1,1), 0, LWA_COLORKEY);
	setbkcolor(RGB(1,1,1));//用不常用颜色作为透明色
	cleardevice();
	//移动窗口至屏幕右下角
	int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	int x = screenWidth - win.width;
	int y = screenHeight - win.height;
	MoveWindow(win.hwnd, x, y, 400, 300, TRUE);
	//窗口置顶
	SetWindowPos(win.hwnd, HWND_TOPMOST, x, y, 400, 300, SWP_NOMOVE);
}

//设置默认图像，用于应对图片无法加载的情况，之后再写
void DrawDefaultImages()
{

}

void LoadImages()
{
	loadimage(&imgNormal, _T("normal.png"), 200, 200, true);
	loadimage(&imgSleep, _T("sleep.png"), 200, 200, true);
	loadimage(&imgClick, _T("click.png"), 200, 200, true);
	loadimage(&imgDrag, _T("drag.png"), 200, 200, true);
	loadimage(&imgLongpress, _T("longpress.png"), 200, 200, true);
	loadimage(&imgKeyboard, _T("keyboard.png"), 200, 200, true);
}
void DrawPet()
{
	IMAGE* currentImg=NULL;
	switch (pet.state)
	{
		case NORMAL:
			currentImg = &imgNormal;
			break;
		case SLEEP:
			currentImg = &imgSleep;
			break;
		case CLICK:
			currentImg = &imgClick;
			break;
		case DRAG:
			currentImg = &imgDrag;
			break;
		case LONGPRESS:
			currentImg = &imgLongpress;
			break;
		case KEYBOARD:
			currentImg = &imgKeyboard;
			break;
	}
	putimage(100, 100, currentImg);
	
}

//自动更新，实现交互后恢复待机状态
void UpdatePet()
{
	pet.stateTimer++;
	switch (pet.state)
	{
		case CLICK:
		case LONGPRESS:
		case DRAG:
		case SLEEP:
		case KEYBOARD:
			if (pet.stateTimer > 60)//状态持续一段时间后就恢复待机
			{
				pet.state = NORMAL;
				pet.stateTimer = 0;
			}
	}
}


//测试函数，正式版删除，按下1-6可显示对应6种图像
void Test()
{
    if (GetAsyncKeyState('1') & 0x8000)
    {
        pet.state = NORMAL;
        pet.stateTimer = 0;
    }

    if (GetAsyncKeyState('2') & 0x8000)
    {
        pet.state = SLEEP;
        pet.stateTimer = 0;
    }

    if (GetAsyncKeyState('3') & 0x8000)
    {
        pet.state = CLICK;
        pet.stateTimer = 0;
    }

    if (GetAsyncKeyState('4') & 0x8000)
    {
        pet.state = LONGPRESS;
        pet.stateTimer = 0;
    }

    if (GetAsyncKeyState('5') & 0x8000)
    {
        pet.state = DRAG;
        pet.stateTimer = 0;
    }

    if (GetAsyncKeyState('6') & 0x8000)
    {
        pet.state = KEYBOARD;
        pet.stateTimer = 0;
    }
}



int main()
{
	InitPet();
	InitWindow();
	LoadImages();
	while (1)
	{
		Test();
		
		DrawPet();
		UpdatePet();
		
		Sleep(8);
	}
	
	closegraph();
	return 0;
}
