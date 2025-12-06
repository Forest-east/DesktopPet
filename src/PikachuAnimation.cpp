#include "PikachuAnimation.h"

// 构造函数
PikachuAnimation::PikachuAnimation(int x, int y)
    : currentFrame(0), isPlaying(false), frameCounter(0), x(x), y(y) {
    // 初始化图片数组
    for (int i = 0; i < 5; i++) {
        frames[i] = IMAGE();
    }
}

// 加载图片 - 绝对路径版本
bool PikachuAnimation::LoadImages() {
    // !!!重要!!! 请修改下面的路径为您的图片实际位置 !!!
    // 如何获取正确路径：
    // 1. 打开您的图片文件夹
    // 2. 右键点击图片 -> 属性 -> 安全 -> 对象名称
    // 3. 复制完整路径，替换下面的路径

    const char* imagePaths[5] = {
        // 示例路径，请根据您的实际情况修改：
        "C:\\Users\\D2330\\Desktop\\TestEasyX\\x64\\Debug\\pikachu_1.png",
        "C:\\Users\\D2330\\Desktop\\TestEasyX\\x64\\Debug\\pikachu_2.png",
        "C:\\Users\\D2330\\Desktop\\TestEasyX\\x64\\Debug\\pikachu_3.png",
        "C:\\Users\\D2330\\Desktop\\TestEasyX\\x64\\Debug\\pikachu_4.png",
        "C:\\Users\\D2330\\Desktop\\TestEasyX\\x64\\Debug\\pikachu_5.png"

        // 如果上面的路径不对，尝试下面的路径（根据您之前的截图）：
        // "C:\\Users\\D2330\\Desktop\\TestEasyX\\TestEasyX\\images\\pikachu_1.png",
        // "C:\\Users\\D2330\\Desktop\\TestEasyX\\TestEasyX\\images\\pikachu_2.png",
        // "C:\\Users\\D2330\\Desktop\\TestEasyX\\TestEasyX\\images\\pikachu_3.png",
        // "C:\\Users\\D2330\\Desktop\\TestEasyX\\TestEasyX\\images\\pikachu_4.png",
        // "C:\\Users\\D2330\\Desktop\\TestEasyX\\TestEasyX\\images\\pikachu_5.png"
    };

    int loadedCount = 0;

    for (int i = 0; i < 5; i++) {
        // 尝试加载图片
        if (loadimage(&frames[i], imagePaths[i]) == 0) {
            loadedCount++;
        }
    }

    // 如果一张都没加载成功，可能是路径不对
    if (loadedCount == 0) {
        // 尝试最后的手段：直接在当前目录查找
        const char* localPaths[5] = {
            "pikachu_1.png",
            "pikachu_2.png",
            "pikachu_3.png",
            "pikachu_4.png",
            "pikachu_5.png"
        };

        for (int i = 0; i < 5; i++) {
            if (loadimage(&frames[i], localPaths[i]) == 0) {
                loadedCount++;
            }
        }
    }

    return (loadedCount == 5);
}

// 更新动画逻辑
void PikachuAnimation::Update() {
    // 如果不播放，直接返回
    if (!isPlaying) return;

    // 增加计数器
    frameCounter++;

    // 每6帧前进一帧（控制播放速度）
    if (frameCounter >= 6) {
        frameCounter = 0;
        currentFrame++;

        // 播放完最后一帧后停止
        if (currentFrame >= 4) {
            isPlaying = false;
            currentFrame = 0; // 重置为第一帧
        }
    }
}

// 渲染当前帧
void PikachuAnimation::Render() {
    // 只在播放时渲染当前帧
    if (isPlaying && frames[currentFrame].getwidth() > 0) {
        putimage(x, y, &frames[currentFrame], SRCCOPY);
    }
    // 不播放时什么都不渲染（保持黑屏）
}

// 开始播放一轮动画
void PikachuAnimation::StartAnimation() {
    isPlaying = true;
    currentFrame = 0;
    frameCounter = 0;
}

// 检查是否正在播放
bool PikachuAnimation::IsPlaying() const {
    return isPlaying;
}

// 设置显示位置
void PikachuAnimation::SetPosition(int x, int y) {
    this->x = x;
    this->y = y;
}