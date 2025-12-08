
#include"Animation.h"

// 4. 主函数
int main()
{
    // 初始化窗口
    initgraph(WIN_WIDTH, WIN_HEIGHT, EW_SHOWCONSOLE);
    

    // 白色背景
    setbkcolor(WHITE);
    setfillcolor(WHITE);
    cleardevice();

    
    LoadImages();

 
    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            printf("\n> 程序退出\n");
            break;
        }

        UpdateAnimation();

        // 双缓冲绘制
        BeginBatchDraw();
        DrawCurrentFrame();
        EndBatchDraw();

        Sleep(30);
    }

    closegraph();
    return 0;
}