#pragma once
#include<stdio.h>
#include<Windows.h>
#include "Define.h"
#include"CGameFrame.h"
#include"CGameResult.h"
#include"inc/fmod.hpp"

#pragma comment(lib, "fmod_vc.lib")

class CMainManager : public CGameFrame {
private:
	HDC hMainBuffer; 
	
	HDC hBackgroundDC;
	HDC hButtonDC;
	HDC hNecoDC;
	HDC hBlackDC;

	GAME_STEP Select; // 리턴값으로 줄 게임 스텝
	GAME_STEP Result;

	DWORD dwCurTime;

	DWORD dwSelect; // 현제 선택되어있는 버튼
	
	DWORD dwNecoTime; // 선택 마커
	DWORD dwNecoAni; // 고양이 에니메이션 순서
	
	DWORD dwButtonTime; // 버튼이 연속으로 이동하는거 방지

	CGameResult Screen;

	FMOD::System* m_pSystem;
	FMOD::Sound* m_pBGM, * m_pSelect, * m_pSelectMove;
	FMOD::Channel* m_pChannel, * m_pChBGM;

public:
	GAME_STEP Update();
	void Render();

	CMainManager(HWND hProcWnd, HDC hBackbuffer);
	~CMainManager();
};