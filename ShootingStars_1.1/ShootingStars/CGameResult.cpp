#include"CGameResult.h"

CGameResult::CGameResult() {
	bResult = false;

	state = RESULT_NOT;

	nName[0] = 65;
	nName[1] = 65;
	nName[2] = 65;
	nameOrder = 0;

	fSelectXPos = 725;
	fSelectYPos = 550;
	fGameResultYPos = 200;
	fBarXPos = 0;
	fBarYPos = 500;
	fScreenXPos = 0;

	dwScore = 0;

	dwMoveTime = 0;
	dwButtonTime = 0;

	// »ç¿îµå ---------------------------------------------------------------------
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);

	m_pSystem->createSound("Resource\\Play\\Select.wav", FMOD_DEFAULT, 0, &m_pSelect);
	m_pSystem->createSound("Resource\\Play\\SelectMove.wav", FMOD_DEFAULT, 0, &m_pSelectMove);
}

CGameResult::~CGameResult() {
	m_pChannel->stop();

	m_pSelect->release();
	m_pSelectMove->release();

	m_pSystem->close();
	m_pSystem->release();
}

void CGameResult::Update(DWORD Curtime, DWORD score) {
	Move(Curtime);
	Selection(Curtime);
	SetScore(score);

	m_pSystem->update();
}

void CGameResult::Move(DWORD Curtime) {
	if (state == RESULT_BLACK) {
		if (Curtime - dwMoveTime > 7) {
			dwMoveTime = Curtime;
			fScreenXPos += 50;
		}
	}
	else {
		if (Curtime - dwMoveTime > 10 && fBarXPos < 1700) {
			dwMoveTime = Curtime;

			fBarXPos += 50;
		}
	}
}

void CGameResult::Selection(DWORD Curtime) {
	if (fBarXPos >= 1700 && Curtime - dwButtonTime > 100) {
		dwButtonTime = Curtime;

		if (GetAsyncKeyState(VK_LEFT) < 0 && nameOrder > 0) {
			nameOrder--;
			fSelectXPos -= 82;

			m_pSystem->playSound(m_pSelectMove, 0, false, &m_pChannel);
		}
		else if (GetAsyncKeyState(VK_RIGHT) < 0 && nameOrder < 2) {
			nameOrder++;
			fSelectXPos += 82;

			m_pSystem->playSound(m_pSelectMove, 0, false, &m_pChannel);
		}
		else if (GetAsyncKeyState(VK_UP) < 0 && nName[nameOrder] < 90) {
			nName[nameOrder]++;

			m_pSystem->playSound(m_pSelectMove, 0, false, &m_pChannel);
		}
		else if (GetAsyncKeyState(VK_DOWN) < 0 && nName[nameOrder] > 65) {
			nName[nameOrder]--;

			m_pSystem->playSound(m_pSelectMove, 0, false, &m_pChannel);
		}
		else if (GetAsyncKeyState(VK_RETURN) < 0) {
			for (int i = 0; i < 3; i++) {
				strName.push_back(nName[i]);
			}

			bResult = true;

			m_pSystem->playSound(m_pSelect, 0, false, &m_pChannel);
		}
	}
}

void CGameResult::SetScore(DWORD dwScore) {
	switch (state) {
		case RESULT_GAME_OVER:
		{
			this->dwScore = dwScore;
			break;
		}
		case RESULT_CLEAR:
		{
			if (dwScore > this->dwScore + 100) {
				this->dwScore += 100;
			}
			else if (dwScore > this->dwScore + 10) {
				this->dwScore += 10;
			}
			else if (dwScore > this->dwScore) {
				this->dwScore++;
			}
			break;
		}
	}
}