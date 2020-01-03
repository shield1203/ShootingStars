#pragma once
#ifdef _WIN32
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif

#include<stdio.h>
#include<Windows.h>
#include<vector>
#include<string>
#include "Define.h"
#include"CGameFrame.h"
#include"CPlayer.h"
#include"CItem.h"
#include"CBackground.h"
#include"CEnemy.h"
#include"CBoss.h"
#include"CGameResult.h"
#include"CRank.h"
#include"inc/fmod.hpp"
#include"CStage.h"
#include"CLoadStage.h"

#pragma comment(lib, "fmod_vc.lib")

class CPlayManager : public CGameFrame{
private:
	HDC hPlayBuffer;

	HDC hInterfaceDC;
	HDC hNumDC;

	HDC hBackgroundDC;
	HDC hBackObjectDC; // ��� ������Ʈ (����)

	HDC hPlayerDC;
	HDC hPlayerSkillDC;
	HDC hPlayerEffectDC;
	HDC hPlayerObjectDC; // �߻�ü
	HDC hPlayerObjectHitDC; // �߻�ü ��Ʈ ����Ʈ

	HDC hBossDC;
	HDC hBossDieDC;
	HDC hEnemyDC;
	HDC hEnemyObjectDC; // �� �߻�ü
	HDC hEnemyDieDC;

	HDC hItemDC;

	HDC hGameClearDC;
	HDC hGameOverDC; // ���ӿ��� ���
	HDC hBlackDC;
	HDC hSelectionDC;
	HDC hRankBarDC;

	RECT rtCheckCollision;

	GAME_STEP GameStep;
	PLAY_STEP PlayStep;
	//STAGE_STEP StageStep;

	DWORD dwCurTime; // ��� ���ŵǴ� �ð�

	DWORD dwCountTime; 
	DWORD dwSecond; // ���� �ð�(��)
	DWORD dwResultTime;
	DWORD dwStageTime;

	bool bStageSet;

	CLoadStage LoadStage;
	std::vector<CStage*> StageInfo;
	int nEnemyCount;

	CBackground *Background;
	CPlayer *Player;
	std::vector<CItem> Item;
	std::vector<CEnemy> Enemy;
	CBoss *Boss;
	CGameResult *GameResult;
	CRank *Rank;

	FMOD::System* m_pSystem;
	FMOD::Sound* m_pNomalBGM, * m_pBossBGM, * m_pWeapon, * m_pItem, * m_pDie, * m_pEnemyDie;
	FMOD::Channel* m_pChannel, *m_pChBGM, * m_pChWeapon;
public:
	void CollisionCheck();
	void StageManage(DWORD dwCurTime);
	GAME_STEP Update();
	void Render();

	CPlayManager(HWND hProcWnd, HDC hBackbuffer);
	~CPlayManager();
};