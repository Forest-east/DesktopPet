#include"Animation.h"


// 全局变量
IMAGE imgFrames[10];                 // 存储图片
AnimState currentState = STATE_IDLE;
int currentFrame = 0;
int delayCounter = 0;
bool keyProcessed = false;

// 1. 加载所有图片 (最简单的方式)
void LoadImages() {
    printf("=========================================\n");
    printf("    加载皮卡丘图片...\n");
    printf("=========================================\n");        

    for (int i = 0; i < TOTAL_FRAMES; i++) {          
        char filename[30];

        sprintf_s(filename, "pikachu_%02d.png", i + 1);

        if (loadimage(&imgFrames[i], filename) != 0) {
            printf("  [成功] %s (%dx%d)\n", 
                filename,
                imgFrames[i].getwidth(),
                imgFrames[i].getheight());
        }
        else {
            printf("  [失败] %s\n", filename);
            printf("      请检查: 1)文件名 2)文件路径\n");
        }
    }
    printf("=========================================\n\n");
}

// 2. 更新动画逻辑
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

// 3. 绘制当前画面 (白色背景)
void DrawCurrentFrame() {
    // 清屏为白色
    cleardevice();

    // 获取当前图片尺寸
    int imgW = imgFrames[currentFrame].getwidth();
    int imgH = imgFrames[currentFrame].getheight();

    // 计算居中坐标
    int drawX = (WIN_WIDTH - imgW) / 2;
    int drawY = (WIN_HEIGHT - imgH) / 2;

    // 使用最简单的3参数putimage (完全兼容)
    putimage(drawX, drawY, &imgFrames[currentFrame]);

    // 状态信息 (深色文字在白色背景上)
    settextcolor(BLUE);
    settextstyle(14, 0, "宋体");

    if (currentState == STATE_PLAYING) {
        char info[50];
        sprintf_s(info, "播放中: %d/%d 帧", currentFrame + 1, TOTAL_FRAMES);
        outtextxy(10, 10, info);
    }
    else {
        outtextxy(10, 10, "待机中 (按空格键)");
    }

    // 操作提示
    settextcolor(BLACK);
    settextstyle(16, 0, "楷体");
    outtextxy(WIN_WIDTH / 2 - 100, WIN_HEIGHT - 60, "短按 SPACE 播放动画");

    // 窗口提示
    settextcolor(DARKGRAY);
    settextstyle(12, 0, "宋体");
    outtextxy(10, WIN_HEIGHT - 25, "按 ESC 退出");
}
