#include"Animation.h"

int main()
{
    // 最简单的初始化
    initgraph(WIN_WIDTH, WIN_HEIGHT, SHOWCONSOLE);
    HWND hwnd = GetHWnd();

    // 移除窗口标题栏
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_CAPTION;        // 移除标题栏
    style &= ~WS_THICKFRAME;     // 移除可调整边框
    SetWindowLong(hwnd, GWL_STYLE, style);

    // 设置窗口位置（居中）
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(hwnd, HWND_TOPMOST,
        (screenWidth - WIN_WIDTH) / 2,
        (screenHeight - WIN_HEIGHT) / 2,
        0, 0, SWP_NOSIZE);

    SetWindowText(hwnd, "皮卡丘");

    // 设置窗口透明
    SetWindowLong(hwnd, GWL_EXSTYLE,
        GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    printf("皮卡丘桌面宠物\n");
    printf("空格键: 播放动画\n");
    printf("ESC键: 退出\n\n");

    LoadImages();

    // 主循环
    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE)) break;

        // 拖动功能
        if (GetAsyncKeyState(VK_LBUTTON)) {
            POINT pt;
            GetCursorPos(&pt);
            SetWindowPos(hwnd, HWND_TOPMOST,
                pt.x - WIN_WIDTH / 2,
                pt.y - WIN_HEIGHT / 2,
                0, 0, SWP_NOSIZE);
        }

        UpdateAnimation();

        BeginBatchDraw();
        cleardevice();  // 清为黑色（黑色会透明）
        DrawCurrentFrame();
        EndBatchDraw();

        Sleep(50);
    }

    closegraph();
    return 0;
}