#pragma once
#include"Define.h"
#include<Windows.h>
#include<iostream>
#include<cstring>
#include"inc/fmod.hpp"

#pragma comment(lib, "fmod_vc.lib")

class CGameResult {
public:
	bool bResult;

	GAME_RESULT state;

	int nName[3];
	short nameOrder; // 몇번째 이름인가 AAA
	std::string strName;

	float fSelectXPos;
	float fSelectYPos;
	float fGameResultYPos;
	float fBarXPos;
	float fBarYPos;
	float fScreenXPos;

	DWORD dwScore;

	DWORD dwMoveTime;
	DWORD dwButtonTime;

	FMOD::System* m_pSystem;
	FMOD::Sound*  m_pSelect, * m_pSelectMove;
	FMOD::Channel* m_pChannel;

	void Update(DWORD Curtime, DWORD score);
	void Move(DWORD Curtime);
	void Selection(DWORD Curtime);
	void SetScore(DWORD dwScore);

	CGameResult();
	~CGameResult();
};