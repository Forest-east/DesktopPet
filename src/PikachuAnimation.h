#pragma once
#include <graphics.h>

class PikachuAnimation {
private:
    IMAGE frames[5];      // 5张图片
    int currentFrame;     // 当前帧索引 (0-4)
    bool isPlaying;       // 是否正在播放
    int frameCounter;     // 帧计数器，控制播放速度
    int x, y;            // 显示位置

public:
    PikachuAnimation(int x = 250, int y = 150);

    // 加载图片 - 使用绝对路径
    bool LoadImages();

    // 更新动画逻辑
    void Update();

    // 渲染当前帧（只有播放时才渲染）
    void Render();

    // 开始播放一轮动画
    void StartAnimation();

    // 检查是否正在播放
    bool IsPlaying() const;

    // 设置显示位置
    void SetPosition(int x, int y);
};