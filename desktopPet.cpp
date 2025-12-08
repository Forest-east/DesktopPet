
#define IMG_COUNT 4

#include <graphics.h>
#include <conio.h>
#include <windows.h>

// 使窗口透明无边框且一直置顶
void SetWindowStyle()
{
    HWND hwnd = GetHWnd();
    SetWindowLong(hwnd, GWL_STYLE,
                  GetWindowLong(hwnd, GWL_STYLE) & ~WS_CAPTION & ~WS_SYSMENU);
    SetWindowLong(hwnd, GWL_EXSTYLE,
                  GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOPMOST);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
}

// 判断鼠标是否按在图像上
bool IsPointInRect(int x, int y, int rx, int ry, int rw, int rh)
{
    return x >= rx && x <= rx + rw &&
           y >= ry && y <= ry + rh;
}

int main()
{
    initgraph(640, 480); // 创建窗口
    SetWindowStyle();

    // 创建图片对象
    IMAGE pet;            // 初始图片，对应默认状态
    IMAGE img[IMG_COUNT]; // 长按状态对应图片

    // 设置图片尺寸及位置
    int imgWidth = 200;
    int imgHeight = 200;
    int imgX = 220;
    int imgY = 140;

    // 加载图片
    loadimage(&pet, L"res/0-.png", imgWidth, imgHeight);
    loadimage(&img[0], L"res/1-.png", imgWidth, imgHeight);
    loadimage(&img[1], L"res/2-.png", imgWidth, imgHeight);
    loadimage(&img[2], L"res/3-.png", imgWidth, imgHeight);
    loadimage(&img[3], L"res/4-.png", imgWidth, imgHeight);

    putimage(imgX, imgY, &pet); // 图像绘制函数

    DWORD pressTime = 0;
    bool isPressed = false;
    bool firstTrigger = false;
    int currentImgIndex = 0;
    DWORD lastSwitchTime = 0;

    const DWORD longPressThreshold = 1000; // 固定长按1s后触发反应
    const DWORD switchInterval = 300;      // 固定切换间隔300ms

    while (true)
    {
        DWORD currentTime = GetTickCount();

        // 检测是否按下鼠标
        if (MouseHit())
        {
            MOUSEMSG m = GetMouseMsg();

            // 按下左键
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                if (IsPointInRect(m.x, m.y, imgX, imgY, imgWidth, imgHeight))
                {
                    pressTime = currentTime;
                    isPressed = true;
                    firstTrigger = false;
                    currentImgIndex = 0;
                }
            }
            // 松开左键
            else if (m.uMsg == WM_LBUTTONUP)
            {
                isPressed = false;
                firstTrigger = false;
                currentImgIndex = 0;
                putimage(imgX, imgY, &pet);
            }
        }

        // 长按检测
        if (isPressed)
        {
            DWORD holdTime = currentTime - pressTime; // 检测长按持续时间

            // 长按超过1s触发长按反应（首次）
            if (!firstTrigger && holdTime >= 1000)
            {
                firstTrigger = true;
                currentImgIndex = 0;
                lastSwitchTime = currentTime + switchInterval;
            }

            // 再次触发（循环长按反应）
            if (firstTrigger && (currentTime - lastSwitchTime >= switchInterval))
            {
                currentImgIndex = (currentImgIndex + 1) % IMG_COUNT;
                lastSwitchTime = currentTime; // 更新切换时间
            }

            putimage(imgX, imgY, &img[currentImgIndex]);
        }
    }

    closegraph();
    return 0;
}
