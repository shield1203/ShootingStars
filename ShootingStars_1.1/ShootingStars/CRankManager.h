#pragma once
#ifdef _WIN32
	#pragma comment(lib, "ws2_32")
	#include <WinSock2.h>
	#include <ws2tcpip.h>
#endif

#include<Windows.h>
#include"Define.h"
#include"CGameFrame.h"
#include"CRank.h"
#include"CGameResult.h"
#include"inc/fmod.hpp"

#pragma comment(lib, "fmod_vc.lib")

class CRankManager : public CGameFrame {
public:
	HDC hRankBuffer;
	HDC hBackgroundDC;
	HDC hWordDC;
	HDC hBlackDC;

	GAME_STEP GameStep;
	RANK_STATE State;

	int dwCount;
	DWORD dwCurTime;
	DWORD dwRankTime;
	DWORD dwSkipTime;

	CRank *Rank;
	CGameResult BackMenu;

	FMOD::System* m_pSystem;
	FMOD::Sound* m_pBGM, * m_pSelect, * m_pMoveList;
	FMOD::Channel* m_pChannel, *m_pChBGM;

	void KeyCheck();
	void RankShow();
	GAME_STEP Update();
	void Render();

	CRankManager(HWND hProcWnd, HDC hBackbuffer);
	~CRankManager();
};