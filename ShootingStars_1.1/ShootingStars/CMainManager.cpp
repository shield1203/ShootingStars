#ifdef _WIN32
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif

#include "CMainManager.h"

CMainManager::CMainManager(HWND hProcWnd, HDC hBackbuffer) {
	hWnd = hProcWnd;
	hMainBuffer = hBackbuffer;

	Select = STEP_MAIN;
	Result = STEP_MAIN;
	dwSelect = SEL_PLAY;

	dwCurTime = 0;

	dwNecoTime = 0;
	dwNecoAni = 0;
	dwButtonTime = 0;

	hBackgroundDC = CreateBmpDC("Resource\\Main\\MainBackground.bmp");
	hButtonDC = CreateBmpDC("Resource\\Main\\Menu.bmp");
	hNecoDC = CreateBmpDC("Resource\\Main\\MainNeco.bmp");
	hBlackDC = CreateBmpDC("Resource\\Play\\Black.bmp");

	// 사운드 ---------------------------------------------------------------------
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);

	m_pSystem->createSound("Resource\\Main\\BGM.mp3", FMOD_LOOP_NORMAL, 0, &m_pBGM);
	m_pSystem->createSound("Resource\\Play\\Select.wav", FMOD_DEFAULT, 0, &m_pSelect);
	m_pSystem->createSound("Resource\\Play\\SelectMove.wav", FMOD_DEFAULT, 0, &m_pSelectMove);

	m_pSystem->playSound(m_pBGM, 0, false, &m_pChBGM);
	m_pChBGM->setVolume(0.5f);
}

CMainManager::~CMainManager() {
	DeleteDC(hBackgroundDC);
	DeleteDC(hButtonDC);
	DeleteDC(hNecoDC);

	m_pChannel->stop();
	m_pChBGM->stop();

	m_pBGM->release();
	m_pSelectMove->release();
	m_pSelect->release();

	m_pSystem->close();
	m_pSystem->release();
}

GAME_STEP CMainManager::Update() {
	dwCurTime = GetTickCount64();
	m_pSystem->update();

	if (dwCurTime - dwNecoTime > 300) { // 고양이
		dwNecoAni %= 9;
		dwNecoAni++;
		dwNecoTime = dwCurTime;
	}

	if (!Screen.bResult) {
		if ((GetAsyncKeyState(VK_UP) < 0) && dwSelect != SEL_PLAY && dwCurTime - dwButtonTime > 150)
		{
			dwSelect--;
			dwButtonTime = dwCurTime;

			m_pSystem->playSound(m_pSelectMove, 0, false, &m_pChannel);
		}
		else if ((GetAsyncKeyState(VK_DOWN) < 0) && dwSelect != SEL_EXIT && dwCurTime - dwButtonTime > 150) {
			dwSelect++;
			dwButtonTime = dwCurTime;

			m_pSystem->playSound(m_pSelectMove, 0, false, &m_pChannel);
		}
		else if (GetAsyncKeyState(VK_RETURN) < 0 && dwCurTime - dwButtonTime > 150) {
			dwButtonTime = dwCurTime;
			switch (dwSelect) {
				case SEL_PLAY:
				{
					Select = STEP_PLAY;
					break;
				}
				case SEL_RANK:
				{
					Select = STEP_RANK;
					break;
				}
				case SEL_EXIT:
				{
					Select = STEP_EXIT;
					break;
				}
			}

			Screen.bResult = true;
			Screen.state = RESULT_BLACK;

			m_pSystem->playSound(m_pSelect, 0, false, &m_pChannel);
		}
	}
	else {
		Screen.Update(dwCurTime, 0);
		if (Screen.fScreenXPos >= 1700) {
			Result = Select;
		}
	}
	return Result;
}

void CMainManager::Render() {
	BitBlt(hMainBuffer, 0, 0, 1699, 799, hBackgroundDC, 0, 0, SRCCOPY);

	TransparentBlt(hMainBuffer, 670, 450 + (dwSelect * 100), 100, 100, hNecoDC, 50 * dwNecoAni, 0, 50, 50, RGB(0, 222, 0));

	for (int i = SEL_PLAY; i <= SEL_EXIT; i++) {
		if (dwSelect == i) {
			TransparentBlt(hMainBuffer, 750, 450 + (i * 100), 200, 100, hButtonDC, 200, i * 100, 200, 100, RGB(0, 222, 0));
		}
		else {
			TransparentBlt(hMainBuffer, 750, 450 + (i * 100), 200, 100, hButtonDC, 0, i * 100, 200, 100, RGB(0, 222, 0));
		}
	}

	if (Screen.state == RESULT_BLACK && Screen.bResult) {
		BitBlt(hMainBuffer, 0, 0, Screen.fScreenXPos, 800, hBlackDC, 0, 0, SRCCOPY);
	}
}