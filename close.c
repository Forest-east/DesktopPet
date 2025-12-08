#include<windows.h>
   LRESULT CALLBACK  WindowsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {
        case WM_CLOSE:return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);

        }
    }
