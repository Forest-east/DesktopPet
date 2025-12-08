#include <raylib.h>
#include<stdio.h>
int main(){
	const int width = 1400;
	const int length = 1400;
	InitWindow(width, length, "×ÀÃæ³èÎï");
	Texture Normal = LoadTexture("resource/imgnormal.jpg");
	Texture Rest = LoadTexture("resource/imgrest.jpg");
	enum PetState { state_normal, state_rest};
	enum PetState current = state_normal;
	float timer = 0.0f;
	const float trigger = 5.0f;
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		if (GetKeyPressed()!=0 || IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			timer = 0.0f;
			current = state_normal;
		}
		else {
			timer += GetFrameTime();
		}
		if (timer >= trigger && current != state_rest)
		{
			current = state_rest;
		}
		BeginDrawing();
		ClearBackground(RAYWHITE);
		if (current == state_normal) {
			DrawTexture(Normal, 0, 0, WHITE);
		}
		else if (current == state_rest) {
			DrawTexture(Rest, 0, 0, WHITE);
		}
		EndDrawing();
	}
	return 0;
}