#include <graphics.h> 
#include <conio.h>    
#include <stdio.h>
#include <time.h>
#include <vector>
#include <string>
#include <sstream>

#define N 9
typedef struct Pet {
    int x, y;
    int width, height;
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
} Pet;

void LoadPetImages(Pet* pet);
void HandleDragging(Pet* pet, MOUSEMSG msg);

int main()
{
    initgraph(640, 480, EW_SHOWCONSOLE);
    setbkcolor(BLACK);

    Pet deskPet;
    deskPet.x = 400;
    deskPet.y = 300;
    deskPet.width = 113;
    deskPet.height = 144;
    deskPet.currentFrame = 0;
    deskPet.frames = 6;
    deskPet.isDragging = false;
    deskPet.offX = 0;
    deskPet.offY = 0;
    deskPet.Speed = 400;
    deskPet.frameDelay = 1000 / 60;

    LoadPetImages(&deskPet);

    // 待机动画相关变量
    static int frameIndex = 0;
    static DWORD lastTime = clock();

    // 帧率控制
  
    const int targetFPS = 60;
    const int frameDelay = 1000 / targetFPS;

    while (true)
    {
 

        // 处理鼠标消息
        if (MouseHit())
        {
            MOUSEMSG msg = GetMouseMsg();
            HandleDragging(&deskPet, msg);
        }

        // 双缓冲开始
        BeginBatchDraw();

        // 清屏
        cleardevice();

        // 根据状态绘制宠物
        if (deskPet.isDragging)
        {
            // 拖拽时显示拖拽图像
            putimage(deskPet.x - deskPet.width / 2,
                deskPet.y - deskPet.height / 2,
                &deskPet.img[1]);
        }
        else
        {
            // 待机时播放待机动画
            DWORD currentTime = clock();
            if (currentTime - lastTime > 1000 / N) {
                frameIndex = (frameIndex + 1) % N;
                lastTime = currentTime;
            }
            putimage(deskPet.x - deskPet.width / 2,
                deskPet.y - deskPet.height / 2,
                &deskPet.relaxImg[frameIndex]);
        }

        EndBatchDraw();

        
    }

    return 0;
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