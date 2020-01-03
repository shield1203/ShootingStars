#include "CProcessManager.h"

CProcessManager::CProcessManager(HWND hWnd) { // 초기화
	GetWindowRect(hWnd, &WinRect);
	hProcWnd = hWnd;

	HBITMAP hBitmap;
	HDC hdc;

	hdc = GetDC(hWnd);
	hBackBuffer = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, WinRect.right - WinRect.left, WinRect.bottom - WinRect.top);
	SelectObject(hBackBuffer, (HBITMAP)hBitmap);
	DeleteObject(hBitmap);
	ReleaseDC(hWnd, hdc);

	gameStep = prevGameStep = STEP_MAIN; // 현재 게임스텝과 이전 게임 스텝

	Main = new CMainManager(hProcWnd, hBackBuffer);
	Play = nullptr;
	Rank = nullptr;
}

CProcessManager::~CProcessManager() {
	DeleteDC(hBackBuffer);
}

void CProcessManager::Process()
{
	Update();
	Render();
}

void CProcessManager::Update() {
	switch (gameStep)
	{
		case STEP_MAIN: {
			gameStep = Main->Update();
			break;
		}
		case STEP_PLAY: {
			gameStep = Play->Update();
			break;
		}
		case STEP_RANK: {
			gameStep = Rank->Update();
			break;
		}
		case STEP_EXIT: {
			gameStep = STEP_EXIT;
			break;
		}
	}

	if (gameStep != prevGameStep) { // 스텝 이동시 동적할당 해제
		switch (prevGameStep)
		{
			case STEP_MAIN: {
				delete Main;
				Main = nullptr;
				break;
			}
			case STEP_PLAY: {
				delete Play;
				Play = nullptr;
				break;
			}
			case STEP_RANK: {
				delete Rank;
				Rank = nullptr;
				break;
			}
		}

		switch (gameStep) {
			case STEP_MAIN: {
				if (Main == nullptr) {
					Main = new CMainManager(hProcWnd, hBackBuffer);
				}
				break;
		}
			case STEP_PLAY: {
				if (Play == nullptr) {
					Play = new CPlayManager(hProcWnd, hBackBuffer);
				}
				break;
			}
			case STEP_RANK: {
				if (Rank == nullptr) {
					Rank = new CRankManager(hProcWnd, hBackBuffer);
				}
				break;
			}
		}
		prevGameStep = gameStep;
	}
}

void CProcessManager::Render() {
	HDC hdc = GetDC(hProcWnd);

	switch (gameStep)
	{
		case STEP_MAIN: {
			Main->Render();
			break;
		}
		case STEP_PLAY: {
			Play->Render();
			break;
		}
		case STEP_RANK: {
			Rank->Render();
			break;
		}
		case STEP_EXIT: {
			SendMessage(hProcWnd, WM_DESTROY, 0, 0);  //  프로그램 종료
			break;
		}
	}

	BitBlt(hdc, 0, 0, WinRect.right - WinRect.left, WinRect.bottom - WinRect.top, hBackBuffer, 0, 0, SRCCOPY);
	ReleaseDC(hProcWnd, hdc);
}