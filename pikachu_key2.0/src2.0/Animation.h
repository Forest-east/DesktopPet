#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H


#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<windows.h>
// ========== 【配置区】 ==========
const int TOTAL_FRAMES=5;         // 图片总张数
const int ANIMATION_SPEED=2 ;       // 动画速度
const int WIN_WIDTH=400 ;           // 窗口宽度
const int WIN_HEIGHT=400 ;          // 窗口高度
// ==============================


// 全局变量
extern IMAGE imgFrames[10];                 // 存储图片
enum AnimState { STATE_IDLE, STATE_PLAYING };
extern AnimState currentState;
extern int currentFrame;
extern int delayCounter;
extern bool keyProcessed;


// 2. 更新动画逻辑
void UpdateAnimation();
// 3. 绘制当前画面 (白色背景)
void DrawCurrentFrame();


// 1. 加载所有图片 (最简单的方式)
void LoadImages();

#endif // !ANIMATION_H

