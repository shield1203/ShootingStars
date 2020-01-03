#include "CPlayManager.h"

CPlayManager::CPlayManager(HWND hProcWnd, HDC hBackbuffer) {
	hWnd = hProcWnd;
	hPlayBuffer = hBackbuffer;
	
	GameStep = STEP_PLAY;
	PlayStep = STEP_GO;

	dwCurTime = 0;
	dwCountTime = 0;
	dwSecond = 0;
	dwResultTime = 0;
	dwStageTime = GetTickCount64();

	nEnemyCount = 0;

	Background = new CBackground();
	Player = new CPlayer();
	Boss = new CBoss();
	GameResult = new CGameResult();
	Rank = new CRank();

	hInterfaceDC = CreateBmpDC("Resource\\Play\\Interface2.bmp");
	hNumDC = CreateBmpDC("Resource\\Play\\num.bmp");

	hBackgroundDC = CreateBmpDC("Resource\\Play\\Background.bmp");
	hBackObjectDC = CreateBmpDC("Resource\\Play\\BackObject.bmp");

	hPlayerDC = CreateBmpDC("Resource\\Play\\Ciel2.bmp");
	hPlayerSkillDC = CreateBmpDC("Resource\\Play\\Skill2.bmp");
	hPlayerEffectDC = CreateBmpDC("Resource\\Play\\Effect1.bmp");
	hPlayerObjectDC = CreateBmpDC("Resource\\Play\\PlayerObject.bmp");
	hPlayerObjectHitDC = CreateBmpDC("Resource\\Play\\Hit.bmp");

	hBossDC = CreateBmpDC("Resource\\Play\\MiniBoss.bmp");
	hBossDieDC = CreateBmpDC("Resource\\Play\\BossDie.bmp");
	hEnemyDC = CreateBmpDC("Resource\\Play\\Enemy.bmp");
	hEnemyObjectDC = CreateBmpDC("Resource\\Play\\Object02.bmp");
	hEnemyDieDC = CreateBmpDC("Resource\\Play\\EnemyDie.bmp");

	hItemDC = CreateBmpDC("Resource\\Play\\Item.bmp");

	hGameClearDC = CreateBmpDC("Resource\\Play\\GameClear.bmp");
	hGameOverDC = CreateBmpDC("Resource\\Play\\GameOver2.bmp");
	hSelectionDC = CreateBmpDC("Resource\\Play\\Selection.bmp");
	hRankBarDC = CreateBmpDC("Resource\\Play\\RankBar.bmp");

	hBlackDC = CreateBmpDC("Resource\\Play\\Black.bmp");

	LoadStage.LoadFile(StageInfo, "Stage1.xml");

	// 사운드 ---------------------------------------------------------------------
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);

	m_pSystem->createStream("Resource\\Play\\NomalBGM.mp3", FMOD_LOOP_NORMAL, 0, &m_pNomalBGM);
	m_pSystem->createStream("Resource\\Play\\BossBGM.mp3", FMOD_LOOP_NORMAL, 0, &m_pBossBGM);
	m_pSystem->createSound("Resource\\Play\\Item.wav", FMOD_DEFAULT, 0, &m_pItem);
	m_pSystem->createSound("Resource\\Play\\Weapon.wav", FMOD_DEFAULT, 0, &m_pWeapon);
	m_pSystem->createSound("Resource\\Play\\Die.wav", FMOD_DEFAULT, 0, &m_pDie);
	m_pSystem->createStream("Resource\\Play\\EnemyDie.wav", FMOD_DEFAULT, 0, &m_pEnemyDie);

	m_pSystem->playSound(m_pNomalBGM, 0, false, &m_pChBGM);
	m_pChBGM->setVolume(0.5f);
}

CPlayManager::~CPlayManager() {
	DeleteDC(hInterfaceDC);
	DeleteDC(hNumDC);

	DeleteDC(hBackgroundDC);
	DeleteDC(hBackObjectDC);

	DeleteDC(hPlayerDC);
	DeleteDC(hPlayerEffectDC);
	DeleteDC(hPlayerObjectDC);
	DeleteDC(hPlayerObjectHitDC);

	DeleteDC(hBossDC);
	DeleteDC(hBossDieDC);
	DeleteDC(hEnemyDC);
	DeleteDC(hEnemyObjectDC);
	DeleteDC(hEnemyDieDC);

	DeleteDC(hItemDC);

	DeleteDC(hGameOverDC);
	DeleteDC(hSelectionDC);
	DeleteDC(hRankBarDC);

	delete Background;
	delete Player;
	delete Boss;
	delete GameResult;

	/*for (int i = 0; i < Enemy.size(); i++) { // 오류.. 일단 보류 , 벡터 메모리 해제
		delete &Enemy[i];
	}*/
	Item.clear();
	Enemy.clear();
	StageInfo.clear();

	delete Rank;

	m_pChannel->stop();
	m_pNomalBGM->release();
	m_pBossBGM->release();
	m_pItem->release();
	m_pEnemyDie->release();

	m_pSystem->close();
	m_pSystem->release();
}

void CPlayManager::CollisionCheck() { // 충돌체크 함수(임시)
	for (int i = 0; i < Item.size(); i++) {
		if (Item[i].bOn) // 아이템
		{
			if (IntersectRect(&rtCheckCollision, &(Item[i].collision), &(Player->collision))) {
				Item[i].bOn = false;
				switch (Item[i].kind)
				{
				case LIFE_UP:
				{
					if (Player->dwLifeCount < 9) {
						Player->dwLifeCount += 1;
					}
					break;
				}
				case POWER_UP:
				{
					if (Player->dwWeaponLV < 2) {
						Player->dwWeaponLV += 1;
					}
					break;
				}
				case SKILL_UP:
				{
					if (Player->dwSkillCount < 9) {
						Player->dwSkillCount += 1;
					}
					break;
				}
				}
				m_pSystem->playSound(m_pItem, 0, false, &m_pChannel);
			}
		}
	}

	if (Player->bSkill) {
		for (int i = 0; i < Enemy.size(); i++) {
			if (Enemy[i].bOn && IntersectRect(&rtCheckCollision, &Enemy[i].collision, &Player->skillCollision)) {
				Enemy[i].nHP = 0;
			}

			for (int j = 0; j < 300; j++) {
				if (Enemy[i].Object[j].bOn && IntersectRect(&rtCheckCollision, &Enemy[i].Object[j].collision, &Player->skillCollision)) {
					Enemy[i].Object[j].bOn = false;
				}
			}
		}

		if (Boss->bOn) {
			if (Boss->State != TYPE_0) {
				Boss->nHP -= 5;
			}

			for (int i = 0; i < 300; i++) {
				if (IntersectRect(&rtCheckCollision, &Boss->Object[i].collision, &Player->skillCollision)) {
					Boss->Object[i].bOn = false;
				}
			}
		}
	}
	else {
		for (int i = 0; i < Enemy.size(); i++) {
			if (Enemy[i].bOn) {
				for (int j = 0; j < 68; j++) {
					if (Player->Weapon[j].On && IntersectRect(&rtCheckCollision, &Enemy[i].collision, &(Player->Weapon[j].area))) {
						Player->Weapon[j].On = false;
						Player->Weapon[j].bCollision = true;
						Enemy[i].nHP -= 10 * (Player->dwWeaponLV + 1);
						if (Enemy[i].nHP <= 0) {
							Player->dwScore += 1021;

							m_pSystem->playSound(m_pEnemyDie, 0, false, &m_pChannel);

							if (i == 0 || i == 3) {
								srand((unsigned)time(NULL));
								int nRandom = rand() % 3;
								bool bItem = false;

								for (int k = 0; k < Item.size(); k++) {
									if (!Item[k].bOn) {
										switch (nRandom) {
										case 0: {
											Item[k].Init(SKILL_UP, Enemy[i].fXPos, Enemy[i].fYPos);
											break;
										}
										case 1: {
											Item[k].Init(LIFE_UP, Enemy[i].fXPos, Enemy[i].fYPos);
											break;
										}
										case 2: {
											Item[k].Init(POWER_UP, Enemy[i].fXPos, Enemy[i].fYPos);
											break;
										}
										}

										bItem = true;
										break;
									}
								}

								if (!bItem) {
									CItem* addItem = new CItem();
									addItem->Init(POWER_UP, Enemy[i].fXPos, Enemy[i].fYPos);

									Item.push_back(*addItem);
								}
							}
						}

						m_pSystem->playSound(m_pWeapon, 0, false, &m_pChWeapon);
						m_pChWeapon->setVolume(0.4f);
					}
				}
			}
		}

		if (Player->state != PLAYER_RESPAWN) {
			for (int i = 0; i < Enemy.size(); i++) {
				for (int j = 0; j < 300; j++) {
					if (Enemy[i].Object[j].bOn) {
						if (Player->state != PLAYER_DIE && IntersectRect(&rtCheckCollision, &Enemy[i].Object[j].collision, &(Player->collision))) {
							Player->state = PLAYER_DIE;
							Player->dwLifeCount--;
							Player->dwWeaponLV = 0;

							m_pSystem->playSound(m_pDie, 0, false, &m_pChannel);
						}
					}
				}
			}
		}

		if (Boss->bOn && Boss->State != TYPE_0) {
			for (int i = 0; i < 68; i++) {
				if (Player->Weapon[i].On && IntersectRect(&rtCheckCollision, &Boss->collision, &(Player->Weapon[i].area))) {
					Player->Weapon[i].On = false;
					Player->Weapon[i].bCollision = true;
					Boss->nHP -= 10 * (Player->dwWeaponLV + 1);
					Player->dwScore += 20;

					m_pSystem->playSound(m_pWeapon, 0, false, &m_pChWeapon);
					m_pChWeapon->setVolume(0.4f);
				}
			}
		}

		if (Player->state != PLAYER_RESPAWN) {
			for (int j = 0; j < 300; j++) {
				if (Boss->Object[j].bOn) {
					if (Player->state != PLAYER_DIE && IntersectRect(&rtCheckCollision, &Boss->Object[j].collision, &(Player->collision))) {
						Player->state = PLAYER_DIE;
						Player->dwLifeCount--;
						Player->dwWeaponLV = 0;

						m_pSystem->playSound(m_pDie, 0, false, &m_pChannel);
					}
				}
			}
		}
	}
}

void CPlayManager::StageManage(DWORD dwCurTime) {
	if (nEnemyCount < StageInfo.size() && dwCurTime - dwStageTime > StageInfo[nEnemyCount]->dwTime) {
		dwStageTime = dwCurTime;
		bool bCheck = false;

		for (int i = 0; i < Enemy.size(); i++) {
			if (!Enemy[i].bOn) {
				bool bClean = true; // 발사된 오브젝트가 전부 소멸됬는지 확인
				for (int j = 0; j < 300; j++) {
					if (Enemy[i].Object[j].bOn) {
						bClean = false;
						break;
					}
				}

				if (bClean) {
					Enemy[i].Init(StageInfo[nEnemyCount]->EnemyType, StageInfo[nEnemyCount]->AttackPattern, StageInfo[nEnemyCount]->MovePattern,
						StageInfo[nEnemyCount]->nHP, StageInfo[nEnemyCount]->nXPos, StageInfo[nEnemyCount]->nYPos);
					bCheck = true;
				}
				break;
			}
		}

		if (!bCheck) {
			CEnemy* addEnemy = new CEnemy();
			addEnemy->Init(StageInfo[nEnemyCount]->EnemyType, StageInfo[nEnemyCount]->AttackPattern, StageInfo[nEnemyCount]->MovePattern,
				StageInfo[nEnemyCount]->nHP, StageInfo[nEnemyCount]->nXPos, StageInfo[nEnemyCount]->nYPos);
			Enemy.push_back(*addEnemy);
		}
		nEnemyCount++;
	}

	if (nEnemyCount == StageInfo.size() && !Boss->bOn && Boss->nHP > 0) {
		bool bBossOn = true;

		for (int i = 0; i < Enemy.size(); i++) {
			if (Enemy[i].bOn) {
				bBossOn = false;
				break;
			}
		}

		if (bBossOn) {
			Boss->bOn = true;
			m_pChBGM->stop();
			m_pSystem->playSound(m_pBossBGM, 0, false, &m_pChBGM);
		}
	}
}

GAME_STEP CPlayManager::Update() {
	dwCurTime = GetTickCount64();
	m_pSystem->update();
	StageManage(dwCurTime);

	if (dwCurTime - dwCountTime >= 1000) { // 시간(초) 구하기
		dwCountTime = dwCurTime;
		dwSecond++;
	}

	Background->Update(dwCurTime);
	Player->Update(dwCurTime);
	for (int i = 0; i < 68; i++) {
		if (Player->Weapon[i].On && !Player->Weapon[i].bCollision) {
			Player->Weapon[i].Move(dwCurTime);
		}
	}

	for (int i = 0; i < Item.size(); i++) {
		Item[i].Update(dwCurTime);
	}

	for (int i = 0; i < Enemy.size(); i++) {
		Enemy[i].Update(dwCurTime, Player->collision.left, Player->collision.top);
	}

	Boss->Update(dwCurTime, Player->collision.left, Player->collision.top);

	//충돌체크--------------------------------------------------------------
	CollisionCheck();
	
	// 게임 스텝관리-----------------------------------------------------------------------------------
	if (PlayStep == STEP_GO) {
		if (Player->dwLifeCount == 0) {
			dwResultTime = dwCurTime;
			PlayStep = STEP_FAIL;
		}
		else if (Boss->nHP <= 0 && dwSecond > 45) {
			dwResultTime = dwCurTime;
			PlayStep = STEP_SUCCESS;
		}
	}

	// GAME_RESULT
	if (PlayStep == STEP_FAIL || PlayStep == STEP_SUCCESS || PlayStep == STEP_RECORD && dwCurTime - dwResultTime > 3000) {
		switch (PlayStep) {
			case STEP_FAIL:
			{
				GameResult->state = RESULT_GAME_OVER;
				break;
			}
			case STEP_SUCCESS:
			{
				GameResult->state = RESULT_CLEAR;
				break;
			}
			case STEP_RECORD:
			{
				GameResult->state = RESULT_BLACK;
				break;
			}
		}
		GameResult->Update(dwCurTime, Player->dwScore);
		if (GameResult->fScreenXPos >= 1700) {
			GameStep = STEP_MAIN;
		}

		if (GameResult->bResult) {
			Rank->RequestInsert(GameResult->strName, std::to_string(Player->dwScore));
			GameResult->bResult = false;
			PlayStep = STEP_RECORD;
		}
	}

	return GameStep;
}

void CPlayManager::Render() {
	// 배경 (무한횡스크롤)-----------------------------------------------------------------------------------------------------------------
	if (Background->dwBackgroundXpos + 1700 < 2500) {
		BitBlt(hPlayBuffer, 0, 0, WIN_RIGHT, WIN_BOTTOM, hBackgroundDC, Background->dwBackgroundXpos, 0, SRCCOPY);
	}
	else {
		BitBlt(hPlayBuffer, 0, 0, WIN_RIGHT - (Background->dwBackgroundXpos + 1700 - 2500), WIN_BOTTOM,
			hBackgroundDC, Background->dwBackgroundXpos, 0, SRCCOPY);
		BitBlt(hPlayBuffer, WIN_RIGHT - (Background->dwBackgroundXpos + 1700 - 2500), 0, WIN_RIGHT, WIN_BOTTOM,
			hBackgroundDC, 0, 0, SRCCOPY);
	}

	// 오브젝트(배경) ---------------------------------------------------------------------------------------------------------------------
	TransparentBlt(hPlayBuffer, 0, 500, 1700 - Background->dwObjectXPos, 300, hBackObjectDC, Background->dwObjectXPos, 400, 1700 - Background->dwObjectXPos, 300, RGB(0, 222, 0));
	TransparentBlt(hPlayBuffer, 1700 - Background->dwObjectXPos, 500, Background->dwObjectXPos, 300, hBackObjectDC, 0, 400, Background->dwObjectXPos, 300, RGB(0, 222, 0));
	
	// 불릿오브젝트-----------------------------------------------------------------------
	for (int i = 0; i < 68; i++) {
		if (Player->Weapon[i].On) {
			TransparentBlt(hPlayBuffer, Player->Weapon[i].nXPos, Player->Weapon[i].nYPos, 35, 30, hPlayerObjectDC, 0, Player->dwWeaponLV * 30, 35, 30, RGB(0, 222, 0));
		}

		if (Player->Weapon[i].bCollision) {
			TransparentBlt(hPlayBuffer, Player->Weapon[i].nXPos - 30, Player->Weapon[i].nYPos - 35, 100, 100, hPlayerObjectHitDC, Player->Weapon[i].dwAni, 0, 192, 192, RGB(0, 0, 0));
		}
	}

	// 이펙트(공격, 스킬)
	if (Player->bSkill) {
		BitBlt(hPlayBuffer, 0, Player->skillCollision.top, 1700, 350, hPlayerSkillDC, 0, 0, SRCCOPY);
	}

	if (Player->Effect->On) {
		TransparentBlt(hPlayBuffer, Player->Effect->nXPos, Player->Effect->nYPos, 256, 256, hPlayerEffectDC, Player->Effect->dwAni, Player->Effect->state, 256, 256, RGB(0, 222, 0));
	}

	// 플레이어 캐릭터
	TransparentBlt(hPlayBuffer, Player->nXPos, Player->nYPos, 256, 256, hPlayerDC, Player->dwAni, Player->state, 256, 256, RGB(0, 222, 0));

	// 이펙트(부스터)
	if (Player->Booster->On) {
		TransparentBlt(hPlayBuffer, Player->Booster->nXPos, Player->Booster->nYPos, 256, 256, hPlayerEffectDC, Player->Booster->dwAni, Player->Booster->state, 256, 256, RGB(0, 222, 0));
	}
	
	// 적
	for (int i = 0; i < Enemy.size(); i++) {
		if (Enemy[i].bOn) {
			if (Enemy[i].Type == MECA_GIRL) {
				TransparentBlt(hPlayBuffer, Enemy[i].fXPos, Enemy[i].fYPos, 150, 100, hEnemyDC, Enemy[i].dwAni, Enemy[i].Type, 150, 100, RGB(0, 222, 0));
			}
			else {
				TransparentBlt(hPlayBuffer, Enemy[i].fXPos, Enemy[i].fYPos, 100, 100, hEnemyDC, Enemy[i].dwAni, Enemy[i].Type, 50, 50, RGB(0, 222, 0));
			}
		}

		if (Enemy[i].bCollision) { // 사망 이펙트
			TransparentBlt(hPlayBuffer, Enemy[i].fXPos - 50.0f, Enemy[i].fYPos - 50.0f, 150, 150, hEnemyDieDC, Enemy[i].dwAni, 0, 100, 100, RGB(0, 222, 0));
		}

		for (int j = 0; j < 300; j++) { // 적 오브젝트
			if (Enemy[i].Object[j].bOn) {
				TransparentBlt(hPlayBuffer, Enemy[i].Object[j].fXPos, Enemy[i].Object[j].fYPos, 20, 20, hEnemyObjectDC, 0, 0, 30, 30, RGB(0, 222, 0));
			}
		}
	}

	// 보스 
	if (Boss->bOn) {
		TransparentBlt(hPlayBuffer, Boss->fXPos, Boss->fYPos, 250, 256, hBossDC, Boss->dwAni, Boss->State, 250, 256, RGB(0, 222, 0));
	}

	for (int i = 0; i < 300; i++) {
		if (Boss->Object[i].bOn) {
			TransparentBlt(hPlayBuffer, Boss->Object[i].fXPos, Boss->Object[i].fYPos, 30, 30, hEnemyObjectDC, 0, 0, 30, 30, RGB(0, 222, 0));
		}
	}

	if (Boss->bCollision) {
		TransparentBlt(hPlayBuffer, Boss->fXPos + 50, Boss->fYPos + 100, 150, 150, hBossDieDC, Boss->dwAni, 0, 150, 150, RGB(0, 222, 0));
	}

	//아이템===========================================
	for (int i = 0; i < Item.size(); i++) {
		if (Item[i].bOn) {
			TransparentBlt(hPlayBuffer, Item[i].nXPos, Item[i].nYPos, 60, 60, hItemDC, Item[i].dwAni, Item[i].kind, 30, 30, RGB(0, 222, 0));
		}
	}

	// 인터페이스---------------------------------------------------------------------------------------------------------------------------
	TransparentBlt(hPlayBuffer, 0, 10, 150, 140, hInterfaceDC, 0, 0, 170, 160, RGB(0, 222, 0)); // 캐릭터
	TransparentBlt(hPlayBuffer, 155, 120, 16, 28, hNumDC, Player->dwLifeCount * 8, 0, 8, 14, RGB(0, 222, 0)); // 남은 라이프

	for (int i = 0; i < Player->dwSkillCount; i++) {
		TransparentBlt(hPlayBuffer, 160 + (i * 30), 60, 30, 30, hItemDC, 0, SKILL_UP, 30, 30, RGB(0, 222, 0)); // 남은 스킬수
	}

	DWORD dwScore = Player->dwScore; // 점수
	short count = 0;
	while (dwScore) { 
		TransparentBlt(hPlayBuffer, 300 - (count * 16), 10, 16, 28, hNumDC, (dwScore % 10) * 8, 0, 8, 14, RGB(0, 222, 0));
		dwScore /= 10;
		count++;
	}

	if (PlayStep == STEP_FAIL) {
		BitBlt(hPlayBuffer, 0, GameResult->fGameResultYPos, GameResult->fBarXPos, 200, hGameOverDC, 0, 0, SRCCOPY); // GAME_OVER
		BitBlt(hPlayBuffer, 0, GameResult->fBarYPos, GameResult->fBarXPos, 200, hRankBarDC, 0, 0, SRCCOPY); // RANK_REGIST

		if (GameResult->fBarXPos >= 1700) {
			for (int i = 0; i < 3; i++) { // Name
				TransparentBlt(hPlayBuffer, 725 + (i * 85), GameResult->fSelectYPos, 80, 80, hNumDC, 8 * (GameResult->nName[i] - 65), 14, 8, 8, RGB(0, 222, 0));
			}
			TransparentBlt(hPlayBuffer, GameResult->fSelectXPos, GameResult->fSelectYPos - 2, 80, 84, hSelectionDC, 0, 0, 100, 100, RGB(0, 222, 0)); // Selection
		}
	}

	if (PlayStep == STEP_SUCCESS) {
		
		BitBlt(hPlayBuffer, 0, GameResult->fBarYPos, GameResult->fBarXPos, 200, hRankBarDC, 0, 0, SRCCOPY); // RANK_REGIST

		if (GameResult->fBarXPos >= 1700) {
			for (int i = 0; i < 3; i++) { // Name
				TransparentBlt(hPlayBuffer, 725 + (i * 85), GameResult->fSelectYPos, 80, 80, hNumDC, 8 * (GameResult->nName[i] - 65), 14, 8, 8, RGB(0, 222, 0));
			}
			TransparentBlt(hPlayBuffer, GameResult->fSelectXPos, GameResult->fSelectYPos - 2, 80, 84, hSelectionDC, 0, 0, 100, 100, RGB(0, 222, 0)); // Selection
		}

		DWORD dwResultScore = GameResult->dwScore; // 점수
		short count = 0;
		while (dwResultScore) {
			TransparentBlt(hPlayBuffer, 900 - (count * 28), 450, 28, 48, hNumDC, (dwResultScore % 10) * 8, 0, 8, 14, RGB(0, 222, 0));
			dwResultScore /= 10;
			count++;
		}

		TransparentBlt(hPlayBuffer, 300, 0, 1150, 530, hGameClearDC, 0, 0, 750, 330, RGB(0, 222, 0)); // STAGE_CLEAR
	}
	
	if (GameResult->state == RESULT_BLACK) {
		BitBlt(hPlayBuffer, 0, 0, GameResult->fScreenXPos, 800, hBlackDC, 0, 0, SRCCOPY);
	}
}