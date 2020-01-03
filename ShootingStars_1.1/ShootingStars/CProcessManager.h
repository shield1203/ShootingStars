#pragma once

#ifdef _WIN32
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif

#include<stdio.h>
#include<Windows.h>
#include "CPlayManager.h"
#include "CMainManager.h"
#include "CRankManager.h"
#include "Define.h"

class CProcessManager {
private:
	HWND hProcWnd;
	HDC hBackBuffer;
	RECT WinRect;

	GAME_STEP gameStep, prevGameStep; // ���� ���ӽ��ܰ� ���� ���� ����
	
	//----------------------------------------------------------- ���ܺ� ��ü��
	CPlayManager *Play; 
	CMainManager *Main;
	CRankManager *Rank;

public:
	void Process();
	void Update();
	void Render();

	CProcessManager(HWND hWnd);
	~CProcessManager();
};