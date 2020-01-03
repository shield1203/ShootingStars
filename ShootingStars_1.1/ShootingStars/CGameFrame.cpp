#include "CGameFrame.h"

HDC CGameFrame::CreateBmpDC(const char* filename) {
	HBITMAP hBitmap;
	HDC hdc = GetDC(hWnd);
	HDC hMemDC = CreateCompatibleDC(hdc);
	hBitmap = (HBITMAP)LoadImage(NULL, TEXT(filename), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	SelectObject(hMemDC, hBitmap);
	DeleteObject(hBitmap);
	ReleaseDC(hWnd, hdc);
	return hMemDC;
}