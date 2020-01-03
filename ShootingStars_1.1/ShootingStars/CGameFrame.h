#pragma once
#include<Windows.h>

class CGameFrame {
public:
	HWND hWnd;
	HDC CreateBmpDC(const char* filename);
};