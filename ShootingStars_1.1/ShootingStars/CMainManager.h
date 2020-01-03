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

	GAME_STEP Select; // ���ϰ����� �� ���� ����
	GAME_STEP Result;

	DWORD dwCurTime;

	DWORD dwSelect; // ���� ���õǾ��ִ� ��ư
	
	DWORD dwNecoTime; // ���� ��Ŀ
	DWORD dwNecoAni; // ����� ���ϸ��̼� ����
	
	DWORD dwButtonTime; // ��ư�� �������� �̵��ϴ°� ����

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