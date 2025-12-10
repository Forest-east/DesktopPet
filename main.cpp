#include <graphics.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <string>
#include <sstream>
#include <conio.h>

#define N 9

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

// ========== 【配置区】 ==========皮卡丘敲键盘
const int TOTAL_FRAMES = 5;         // 图片总张数
const int ANIMATION_SPEED = 2;       // 动画速度

// 全局变量皮卡丘敲键盘
IMAGE imgFrames[10];               // 存储图片
enum AnimState { STATE_IDLE, STATE_PLAYING };
AnimState currentState;
int currentFrame;
int delayCounter;
bool keyProcessed;
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
	loadimage(&pet->img[0], _T("IMG_Default.png"), pet->width, pet->height);
	loadimage(&pet->img[1], _T("IMG_IsDragging.png"), pet->width, pet->height);

	for (int index = 0; index < N; index++)
	{
		wchar_t path[100] = { 0 };
		swprintf_s(path, L"Frame%d.png", index + 1);
		loadimage(&pet->relaxImg[index], path, pet->width, pet->height);
	}

	for (int i = 0; i < TOTAL_FRAMES; i++) {
		wchar_t  filename[30];

		swprintf_s(filename, L"pikachu_%02d.png", i + 1);

		loadimage(&imgFrames[i], filename);  
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

// 1. 加载所有pikachu敲键盘图片 


// 2. 更新pikachu敲键盘动画逻辑
void UpdateAnimation() {
	bool spaceKeyDown = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

	if (currentState == STATE_IDLE) {
		if (spaceKeyDown && !keyProcessed) {
			currentState = STATE_PLAYING;
			currentFrame = 0;
			delayCounter = 0;
			keyProcessed = true;
			printf("> 开始播放动画\n");
		}
	}
	else if (currentState == STATE_PLAYING) {
		delayCounter++;

		if (delayCounter >= ANIMATION_SPEED) {
			delayCounter = 0;
			currentFrame++;

			if (currentFrame >= TOTAL_FRAMES) {
				currentState = STATE_IDLE;
				currentFrame = 0;
				printf("> 动画播放完毕\n");
			}
		}
	}

	if (!spaceKeyDown) {
		keyProcessed = false;
	}
}

// 3. 绘制pikachu敲键盘当前画面 (白色背景)
void DrawCurrentFrame() {
	// 清屏为白色
	cleardevice();

	// 获取当前图片尺寸
	int imgW = imgFrames[currentFrame].getwidth();
	int imgH = imgFrames[currentFrame].getheight();
	     
	// 跟随
	int drawX = pet.x;
	int drawY = pet.y;

	// 使用最简单的3参数putimage (完全兼容)
	putimage(drawX, drawY, &imgFrames[currentFrame]);
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






int main()
{
	InitPet();
	InitWindow();

	LoadPetImages(&pet);
	
	while (true)
	{


		// 处理鼠标消息
		if (MouseHit())
		{
			MOUSEMSG msg = GetMouseMsg();
			HandleDragging(&pet, msg);
		}
		

		// 双缓冲开始 
		UpdateAnimation();   
		BeginBatchDraw();  
		 

		// 清屏
		cleardevice();
		DrawCurrentFrame();                
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
	}
	
	

	closegraph();
	return 0;
}

