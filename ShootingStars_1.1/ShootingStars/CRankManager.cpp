#include"CRankManager.h"

CRankManager::CRankManager(HWND hProcWnd, HDC hBackbuffer) {
	hWnd = hProcWnd;
	hRankBuffer = hBackbuffer;

	hBackgroundDC = CreateBmpDC("Resource\\Rank\\RankPage.bmp");
	hWordDC = CreateBmpDC("Resource\\Rank\\num.bmp");
	hBlackDC = CreateBmpDC("Resource\\Play\\Black.bmp");

	GameStep = STEP_RANK;
	State = RANK_WORk;

	Rank = new CRank();

	dwCount = 0;
	dwCurTime = 0;
	dwRankTime = 0;
	dwSkipTime = GetTickCount64();

	// 사운드 ---------------------------------------------------------------------
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);

	m_pSystem->createSound("Resource\\Rank\\BGM.mp3", FMOD_LOOP_NORMAL, 0, &m_pBGM);
	m_pSystem->createSound("Resource\\Play\\Select.wav", FMOD_DEFAULT, 0, &m_pSelect);
	m_pSystem->createSound("Resource\\Rank\\MoveList.wav", FMOD_DEFAULT, 0, &m_pMoveList);

	m_pSystem->playSound(m_pBGM, 0, false, &m_pChBGM);
	m_pChBGM->setVolume(0.5f);
}

CRankManager::~CRankManager() {
	DeleteDC(hBackgroundDC);
	DeleteDC(hWordDC);
	DeleteDC(hBlackDC);

	delete Rank;

	m_pChannel->stop();
	m_pChBGM->stop();

	m_pBGM->release();
	m_pSelect->release();
	m_pMoveList->release();

	m_pSystem->close();
	m_pSystem->release();
}

void CRankManager::KeyCheck() {
	if (GetAsyncKeyState(VK_SPACE) < 0 && dwCurTime - dwSkipTime > 100) { // 위 이동
		dwSkipTime = dwCurTime;
		if (State == RANK_WORk) {
			for (int i = 0; i < 10; i++) {
				Rank->Ranking[i]->fYPos = 180.0f + (i * 60);
			}
			dwCount = 9;
			State = RANK_DONE;
		}
		else {
			BackMenu.bResult = true;
			BackMenu.state = RESULT_BLACK;
		}

		m_pSystem->playSound(m_pSelect, 0, false, &m_pChannel);
	}
}

void CRankManager::RankShow() {
	if (dwCurTime - dwRankTime > 5) {
		dwRankTime = dwCurTime;

		Rank->Ranking[dwCount]->fYPos -= 20.0f;

		if (Rank->Ranking[dwCount]->fYPos <= 180.0f + (dwCount * 60)) {
			dwCount++;

			m_pSystem->playSound(m_pMoveList, 0, false, &m_pChannel);
		}

		if (dwCount == 10) {
			dwCount = 9;
			State = RANK_DONE;
		}
	}
}

GAME_STEP CRankManager::Update() {
	dwCurTime = GetTickCount64();

	if (Rank->Ranking.size() == 0) {
		Rank->RequestList();
	}
	else {
		KeyCheck(); // 키입력 검사

		if (State == RANK_WORk) {
			RankShow();
		}
	}

	if (BackMenu.bResult) {
		BackMenu.Update(dwCurTime, 0);
		if (BackMenu.fScreenXPos >= 1700) {
			GameStep = STEP_MAIN;
		}
	}

	return GameStep;
}

void CRankManager::Render() {
	BitBlt(hRankBuffer, 0, 0, WIN_RIGHT, WIN_BOTTOM, hBackgroundDC, 0, 0, SRCCOPY);

	if (Rank->Ranking.size() != 0) {
		for (int i = 0; i <= dwCount; i++) {
			switch (i) { // 순위
			case 0: {
				TransparentBlt(hRankBuffer, 550, Rank->Ranking[i]->fYPos, 24, 42, hWordDC, 8 * (i + 1), 0, 8, 14, RGB(0, 222, 0)); // 1
				TransparentBlt(hRankBuffer, 575, Rank->Ranking[i]->fYPos + 17, 25, 25, hWordDC, 8 * 18, 14, 8, 8, RGB(0, 222, 0)); // ST
				TransparentBlt(hRankBuffer, 600, Rank->Ranking[i]->fYPos + 17, 25, 25, hWordDC, 8 * 19, 14, 8, 8, RGB(0, 222, 0));
				break;
			}
			case 1: {
				TransparentBlt(hRankBuffer, 550, Rank->Ranking[i]->fYPos, 24, 42, hWordDC, 8 * (i + 1), 0, 8, 14, RGB(0, 222, 0)); // 2
				TransparentBlt(hRankBuffer, 575, Rank->Ranking[i]->fYPos + 17, 25, 25, hWordDC, 8 * 13, 14, 8, 8, RGB(0, 222, 0)); //ND
				TransparentBlt(hRankBuffer, 600, Rank->Ranking[i]->fYPos + 17, 25, 25, hWordDC, 8 * 3, 14, 8, 8, RGB(0, 222, 0));
				break;
			}
			case 2: {
				TransparentBlt(hRankBuffer, 550, Rank->Ranking[i]->fYPos, 24, 42, hWordDC, 8 * (i + 1), 0, 8, 14, RGB(0, 222, 0));
				TransparentBlt(hRankBuffer, 575, Rank->Ranking[i]->fYPos + 17, 25, 25, hWordDC, 8 * 17, 14, 8, 8, RGB(0, 222, 0));
				TransparentBlt(hRankBuffer, 600, Rank->Ranking[i]->fYPos + 17, 25, 25, hWordDC, 8 * 3, 14, 8, 8, RGB(0, 222, 0));
				break;
			}
			case 9: { // 10은 같은 공간에 2자
				TransparentBlt(hRankBuffer, 548, Rank->Ranking[i]->fYPos, 16, 42, hWordDC, 8, 0, 8, 14, RGB(0, 222, 0));
				TransparentBlt(hRankBuffer, 560, Rank->Ranking[i]->fYPos, 16, 42, hWordDC, 0, 0, 8, 14, RGB(0, 222, 0));
				TransparentBlt(hRankBuffer, 578, Rank->Ranking[i]->fYPos + 17, 25, 25, hWordDC, 8 * 19, 14, 8, 8, RGB(0, 222, 0)); // th
				TransparentBlt(hRankBuffer, 598, Rank->Ranking[i]->fYPos + 17, 25, 25, hWordDC, 8 * 7, 14, 8, 8, RGB(0, 222, 0));
				break;
			}
			default: {
				TransparentBlt(hRankBuffer, 550, Rank->Ranking[i]->fYPos, 24, 42, hWordDC, 8 * (i + 1), 0, 8, 14, RGB(0, 222, 0));
				TransparentBlt(hRankBuffer, 575, Rank->Ranking[i]->fYPos + 17, 25, 25, hWordDC, 8 * 19, 14, 8, 8, RGB(0, 222, 0));
				TransparentBlt(hRankBuffer, 595, Rank->Ranking[i]->fYPos + 17, 25, 25, hWordDC, 8 * 7, 14, 8, 8, RGB(0, 222, 0));
				break;
			}
			}

			DWORD dwScore = Rank->Ranking[i]->nScore; // 점수
			short count = 0;
			while (dwScore) {
				TransparentBlt(hRankBuffer, 970 - (count * 24), Rank->Ranking[i]->fYPos, 24, 42, hWordDC, (dwScore % 10) * 8, 0, 8, 14, RGB(0, 222, 0));
				dwScore /= 10;
				count++;
			}

			for (int j = 0; j < 3; j++) {
				int nNum = Rank->Ranking[i]->name[j];
				TransparentBlt(hRankBuffer, 1100 + (j * 42), Rank->Ranking[i]->fYPos, 42, 42, hWordDC, 8 * (nNum - 65), 14, 8, 8, RGB(0, 222, 0));
			}
		}
	}

	if (BackMenu.state == RESULT_BLACK && BackMenu.bResult) {
		BitBlt(hRankBuffer, 0, 0, BackMenu.fScreenXPos, 800, hBlackDC, 0, 0, SRCCOPY);
	}
}