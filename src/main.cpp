// 白色窗口版本 
#include "PikachuAnimation.h"
#include <conio.h>

int main() {
    // 1. 初始化图形窗口
    initgraph(800, 600);

    // 2. 创建皮卡丘动画对象
    PikachuAnimation pikachu(250, 150);

    // 3. 尝试加载图片
    if (!pikachu.LoadImages()) {
        // 加载失败，简单提示后退出
        closegraph();
        return -1;
    }

    // 4. 主循环变量（防止按键重复触发）
    bool spaceKeyPressed = false;
    bool escKeyPressed = false;

    // 5. 主循环
    while (true) {
        // 5.1 检测ESC键（退出程序）
        bool escNowPressed = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;
        if (escNowPressed && !escKeyPressed) {
            break;
        }
        escKeyPressed = escNowPressed;

        // 5.2 检测空格键（单次触发播放）
        bool spaceNowPressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
        if (spaceNowPressed && !spaceKeyPressed) {
            if (!pikachu.IsPlaying()) {
                pikachu.StartAnimation();
            }
        }
        spaceKeyPressed = spaceNowPressed;

        // 5.3 清屏为白色背景
        setbkcolor(WHITE);
        cleardevice();

        // 5.4 更新动画状态
        pikachu.Update();

        // 5.5 渲染动画
        pikachu.Render();

        // 5.6 控制帧率（快速响应）
        Sleep(30);
    }

    // 6. 关闭图形窗口
    closegraph();
    return 0;
}