#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <string>
#include <sstream>

#define N 9
#define IMG_COUNT 4

typedef enum
{
	NORMAL,
	SLEEP,
	CLICK,
	LONGPRESS,
	DRAG,
	KEYBOARD
}PetState;

typedef struct Pet
{
	int x, y;
	int height, width;
	PetState state;
	int stateTimer;
	IMAGE img[2];
	int currentFrame;
	int frames;
	int frameStart;
	int frameTime;
	int frameDelay;
	int Speed;
	int offX;
	int offY;
	IMAGE relaxImg[N];
	bool isRelaxing;
	bool isDragging;
}Pet;

typedef struct Window
{
    int height, width;
    HWND hwnd;
}Window;

Window win;
Pet pet;
IMAGE imgNormal;
IMAGE imgSleep;
IMAGE imgClick;
IMAGE imgDrag;
IMAGE imgLongpress;
IMAGE imgKeyboard;

// 待机动画相关变量
static int frameIndex = 0;
static DWORD lastTime = clock();

// 帧率控制

const int targetFPS = 60;
const int frameDelay = 1000 / targetFPS;


void GetScreenSize(int* width,int* height)
{
	*width = GetSystemMetrics(SM_CXSCREEN);
	*height = GetSystemMetrics(SM_CYSCREEN);
}

void InitPet()
{
	GetScreenSize(&win.width, &win.height);
	pet.width = 113;
	pet.height = 144;
	pet.x = win.width - pet.width;
	pet.y = win.height - pet.height;
	pet.state = NORMAL;
	pet.stateTimer = 0;
	pet.currentFrame = 0;
	pet.frames = 6;
	pet.isDragging = false;
	pet.offX = 0;
	pet.offY = 0;
	pet.Speed = 400;
	pet.frameDelay = 1000 / 60;

}

void InitWindow()
{
	GetScreenSize(&win.width, &win.height);
	initgraph(win.width, win.height, EX_NOCLOSE | EX_NOMINIMIZE);
	win.hwnd = GetHWnd();
	//窗口置顶
	SetWindowPos(win.hwnd, HWND_TOPMOST, 0, 0, win.width, win.height, SWP_NOMOVE);
	//无边框+透明
	SetWindowLong(win.hwnd, GWL_STYLE,
		GetWindowLong(win.hwnd, GWL_STYLE) & ~WS_CAPTION & ~WS_SYSMENU);
	SetWindowLong(win.hwnd, GWL_EXSTYLE,
		GetWindowLong(win.hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOPMOST);
	SetLayeredWindowAttributes(win.hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
}

void LoadPetImages(Pet* pet)
{
	//初始状态
	loadimage(&pet->img[0], _T("IMG_Default.png"), pet->width, pet->height);
	loadimage(&pet->img[1], _T("IMG_IsDragging.png"), pet->width, pet->height);

	//长按状态
	loadimage(&pet->img[2], L"res/0-.png", imgWidth, imgHeight);
	loadimage(&pet->img[3], L"res/1-.png", imgWidth, imgHeight);
	loadimage(&pet->img[4], L"res/2-.png", imgWidth, imgHeight);
	loadimage(&pet->img[5], L"res/3-.png", imgWidth, imgHeight);
	loadimage(&pet->img[6], L"res/4-.png", imgWidth, imgHeight);

	//拖动状态
	for (int index = 0; index < N; index++)
	{
		wchar_t path[100] = { 0 };
		swprintf_s(path, L"Frame%d.png", index + 1);
		loadimage(&pet->relaxImg[index], path, pet->width, pet->height);
	}
}

void HandleDragging(Pet* pet, MOUSEMSG msg)
{
	switch (msg.uMsg)
	{
	case WM_LBUTTONDOWN:
	{
		// 修正：计算宠物的矩形区域
		int left = pet->x - pet->width / 2;
		int right = pet->x + pet->width / 2;
		int top = pet->y - pet->height / 2;    // 注意：这是上边界（y值小）
		int bottom = pet->y + pet->height / 2; // 注意：这是下边界（y值大）

		// 检查鼠标是否在宠物区域内
		if (msg.x >= left && msg.x <= right &&
			msg.y >= top && msg.y <= bottom)
		{
			pet->isDragging = true;
			pet->offX = msg.x - pet->x;
			pet->offY = msg.y - pet->y;
		}
		break;
	}

	case WM_MOUSEMOVE:
	{
		if (pet->isDragging)
		{
			// 更新宠物位置，减去偏移量使得鼠标在点击位置
			pet->x = msg.x - pet->offX;
			pet->y = msg.y - pet->offY;
		}
		break;
	}

	case WM_LBUTTONUP:
	{
		if (pet->isDragging)
		{
			pet->isDragging = false;
		}
		break;
	}
	}
}

void HandleLongPress(
	bool* isPressed,
	bool* firstTrigger,
	DWORD* pressTime,
	DWORD* lastSwitchTime,
	int* currentImgIndex,
	int imgX, int imgY,
	int imgWidth, int imgHeight,
	IMAGE* pet,
	IMAGE img[]
)
{
	const DWORD longPressThreshold = 1000;
	const DWORD switchInterval = 300;
	DWORD currentTime = GetTickCount();

	// 检测鼠标消息
	if (MouseHit())
	{
		MOUSEMSG m = GetMouseMsg();

		// 鼠标按下
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (IsPointInRect(m.x, m.y, imgX, imgY, imgWidth, imgHeight))
			{
				*pressTime = currentTime;
				*isPressed = true;
				*firstTrigger = false;
				*currentImgIndex = 0;
			}
		}
		// 鼠标松开
		else if (m.uMsg == WM_LBUTTONUP)
		{
			*isPressed = false;
			*firstTrigger = false;
			*currentImgIndex = 0;

			putimage(imgX, imgY, pet);
		}
	}

	// 长按检测
	if (*isPressed)
	{
		DWORD holdTime = currentTime - *pressTime;

		// 触发第一次“长按触发点”
		if (!*firstTrigger && holdTime >= longPressThreshold)
		{
			*firstTrigger = true;
			*currentImgIndex = 0;
			*lastSwitchTime = currentTime + switchInterval;
		}

		// 循环长按动画
		if (*firstTrigger && (currentTime - *lastSwitchTime >= switchInterval))
		{
			*currentImgIndex = (*currentImgIndex + 1) % IMG_COUNT;
			*lastSwitchTime = currentTime;
		}

		putimage(imgX, imgY, &img[*currentImgIndex]);
	}
}


//void LoadImages()
//{
//	loadimage(&imgNormal, _T("normal.png"), pet.width, pet.height, true);
//	loadimage(&imgSleep, _T("sleep.png"), pet.width, pet.height, true);
//	loadimage(&imgClick, _T("click.png"), pet.width, pet.height, true);
//	loadimage(&imgDrag, _T("drag.png"), pet.width, pet.height, true);
//	loadimage(&imgLongpress, _T("longpress.png"), pet.width, pet.height, true);
//	loadimage(&imgKeyboard, _T("keyboard.png"), pet.width, pet.height, true);
//}
//void DrawPet()
//{
//	IMAGE* currentImg=NULL;
//	switch (pet.state)
//	{
//		case NORMAL:
//			currentImg = &imgNormal;
//			break;
//		case SLEEP:
//			currentImg = &imgSleep;
//			break;
//		case CLICK:
//			currentImg = &imgClick;
//			break;
//		case DRAG:
//			currentImg = &imgDrag;
//			break;
//		case LONGPRESS:
//			currentImg = &imgLongpress;
//			break;
//		case KEYBOARD:
//			currentImg = &imgKeyboard;
//			break;
//	}
//	putimage(pet.x, pet.y, currentImg);
//	
//}
//
////自动更新，实现交互后恢复待机状态
//void UpdatePet()
//{
//	pet.stateTimer++;
//	switch (pet.state)
//	{
//		case CLICK:
//		case LONGPRESS:
//		case DRAG:
//		case SLEEP:
//		case KEYBOARD:
//			if (pet.stateTimer > 60)//状态持续一段时间后就恢复待机
//			{
//				pet.state = NORMAL;
//				pet.stateTimer = 0;
//			}
//	}
//}


////测试函数，正式版删除，按下1-6可显示对应6种图像
//void Test()
//{
//    if (GetAsyncKeyState('1') & 0x8000)
//    {
//        pet.state = NORMAL;
//        pet.stateTimer = 0;
//    }
//
//    if (GetAsyncKeyState('2') & 0x8000)
//    {
//        pet.state = SLEEP;
//        pet.stateTimer = 0;
//    }
//
//    if (GetAsyncKeyState('3') & 0x8000)
//    {
//        pet.state = CLICK;
//        pet.stateTimer = 0;
//    }
//
//    if (GetAsyncKeyState('4') & 0x8000)
//    {
//        pet.state = LONGPRESS;
//        pet.stateTimer = 0;
//    }
//
//    if (GetAsyncKeyState('5') & 0x8000)
//    {
//        pet.state = DRAG;
//        pet.stateTimer = 0;
//    }
//
//    if (GetAsyncKeyState('6') & 0x8000)
//    {
//        pet.state = KEYBOARD;
//        pet.stateTimer = 0;
//    }
//}



int main()
{
	InitPet();
	InitWindow();

	LoadPetImages(&pet);

	//为长按状态创建
	DWORD pressTime = 0;
	bool isPressed = false;
	bool firstTrigger = false;
	int currentImgIndex = 0;
	DWORD lastSwitchTime = 0;


	while (true)
	{
		DWORD currentTime = GetTickCount();

		// 处理鼠标消息
		if (MouseHit())
		{
			MOUSEMSG msg = GetMouseMsg();
			HandleDragging(&pet, msg);
		}

		// 双缓冲开始
		BeginBatchDraw();

		// 清屏
		cleardevice();

		// 根据状态绘制宠物
		if (pet.isDragging)
		{
			// 拖拽时显示拖拽图像
			putimage(pet.x - pet.width / 2,
				pet.y - pet.height / 2,
				&pet.img[1]);
		}
		else
		{
			// 待机时播放待机动画
			DWORD currentTime = clock();
			if (currentTime - lastTime > 1000 / N) {
				frameIndex = (frameIndex + 1) % N;
				lastTime = currentTime;
			}
			putimage(pet.x - pet.width / 2,
				pet.y - pet.height / 2,
				&pet.relaxImg[frameIndex]);
		}
		EndBatchDraw();


		HandleLongPress(
			&isPressed,
			&firstTrigger,
			&pressTime,
			&lastSwitchTime,
			&currentImgIndex,
			imgX, imgY,
			imgWidth, imgHeight,
			&pet,
			img
		);

	}
	
	closegraph();
	return 0;
}

