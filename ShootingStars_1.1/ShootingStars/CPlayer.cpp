#include"CPlayer.h"

CPlayer::CPlayer() {
	prevState = PLAYER_IDLE;
	state = PLAYER_IDLE;

	dwLifeCount = 9; // Max = 3
	dwSkillCount = 2; // Max = 2
	dwWeaponLV = 0; // Max = 2
	dwWeaponTime = 0;
	dwDieTime = 0;
	dwRespawnTime = 0;
	dwRespawnCount = 0;
	dwSkillTime = 0;

	dwAniTime = 0;
	dwAni = 0;

	dwScore = 1;

	nXPos = 0;
	nYPos = 100;

	bSkill = false;

	collision.left = 116; // 플레이어 충돌값
	collision.right = 185;
	collision.top = 255;
	collision.bottom = 287;

	skillCollision.left = 5;
	skillCollision.right = 1680;

	Booster = new CEffect(EFFECT_BOOSTER);
	Effect = new CEffect(EFFECT_ATTACK);

	// 사운드 -----------------------------------
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);

	m_pSystem->createSound("Resource\\Play\\Respawn.wav", FMOD_DEFAULT, 0, &m_pRespawn);
	m_pSystem->createSound("Resource\\Play\\Attack.wav", FMOD_DEFAULT, 0, &m_pAttack);
	m_pSystem->createSound("Resource\\Play\\Skill.wav", FMOD_DEFAULT, 0, &m_pSkill);
	m_pSystem->createSound("Resource\\Play\\SkillEffect.wav", FMOD_DEFAULT, 0, &m_pSkillEffect);

	m_pSystem->playSound(m_pRespawn, 0, false, &m_pChannel);
}

CPlayer::~CPlayer() {
	m_pChannel->stop();
	m_pChAttack->stop();
	m_pChSkill->stop();

	m_pAttack->release();
	m_pRespawn->release();
	m_pSkill->release();
	m_pSkillEffect->release();

	m_pSystem->close();
	m_pSystem->release();
}

void CPlayer::Update(DWORD dwCurTime) {
	m_pSystem->update();

	if (state != PLAYER_DIE && state != PLAYER_RESPAWN && state != PLAYER_SKILL) {
		Action(dwCurTime);
	}
	else if(state == PLAYER_DIE){
		m_pChSkill->stop();
		Die(dwCurTime);
	}
	else if (state == PLAYER_SKILL) {
		Skill(dwCurTime);
	}
	else {
		Respawn(dwCurTime);
	}

	if (prevState != state) {
		prevState = state;
		dwAni = 0;
	}

	SetEffect(dwCurTime);

	if (dwCurTime - dwAniTime >= 100) {
		dwAniTime = dwCurTime;
		SetAni();
	}
}

void CPlayer::Action(DWORD dwCurTime) {
	if (GetAsyncKeyState(VK_SPACE) < 0) { // 공격(최우선)
		state = PLAYER_ATTACK;
		
		if (GetAsyncKeyState(VK_RIGHT) < 0 && nXPos + 240 < 1700) {
			nXPos += MOVE_SPEED;
			SetCollision(MOVE_RIGHT);

			if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) {
				nYPos -= MOVE_SPEED;
				SetCollision(MOVE_UP);
			}
			else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
				nYPos += MOVE_SPEED;
				SetCollision(MOVE_DOWN);
			}
		}
		else if (GetAsyncKeyState(VK_LEFT) < 0 && nXPos > -85) {
			nXPos -= MOVE_SPEED;
			SetCollision(MOVE_LEFT);

			if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) { // 왜 안되는지 모르겠음
				nYPos -= MOVE_SPEED;
				SetCollision(MOVE_UP);
			}
			else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
				nYPos += MOVE_SPEED;
				SetCollision(MOVE_DOWN);
			}
		}
		else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
			nYPos += MOVE_SPEED;
			SetCollision(MOVE_DOWN);
		}
		else if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) {
			nYPos -= MOVE_SPEED;
			SetCollision(MOVE_UP);
		}
		
		if (dwCurTime - dwWeaponTime >= 50) {
			dwWeaponTime = dwCurTime;
			Attack();
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT) < 0) { // 오른이동
		state = PLAYER_RIGHT;

		if (nXPos + 240 < 1700) {
			nXPos += MOVE_SPEED;
			SetCollision(MOVE_RIGHT);
		}

		if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) {
			nYPos -= MOVE_SPEED;
			SetCollision(MOVE_UP);
		}
		else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
			nYPos += MOVE_SPEED;
			SetCollision(MOVE_DOWN);
		}
	}
	else if (GetAsyncKeyState(VK_LEFT) < 0) { // 왼쪽 이동
		state = PLAYER_LEFT;

		if (nXPos > -85) {
			nXPos -= MOVE_SPEED;
			SetCollision(MOVE_LEFT);
		}
		
		if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) {
			nYPos -= MOVE_SPEED;
			SetCollision(MOVE_UP);
		}
		else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
			nYPos += MOVE_SPEED;
			SetCollision(MOVE_DOWN);
		}
	}
	else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) { // 아래 이동
		state = PLAYER_IDLE;

		nYPos += MOVE_SPEED;
		SetCollision(MOVE_DOWN);
	}
	else if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) { // 위 이동
		state = PLAYER_IDLE;

		nYPos -= MOVE_SPEED;
		SetCollision(MOVE_UP);
	}
	else if (GetAsyncKeyState(KEY_N) < 0) {
		if (dwSkillCount > 0) {
			dwSkillCount--;
			state = PLAYER_SKILL;

			m_pSystem->playSound(m_pSkillEffect, 0, false, &m_pChannel);
			m_pSystem->playSound(m_pSkill, 0, false, &m_pChSkill);
		}
	}
	else {
		state = PLAYER_IDLE;
	}
}

void CPlayer::Die(DWORD dwCurTime) {
	if (dwCurTime - dwDieTime > 40 && nYPos < 800) {
		dwDieTime = dwCurTime;
		nXPos -= 15;
		nYPos += 20;
	}

	if (nYPos >= 800 && dwLifeCount > 0) {
		dwSkillCount = 2;
		state = PLAYER_RESPAWN;

		nXPos = 0;
		nYPos = 100;

		collision.left = 116; 
		collision.right = 185;
		collision.top = 255;
		collision.bottom = 287;

		m_pSystem->playSound(m_pRespawn, 0, false, &m_pChannel);
	}
}

void CPlayer::Skill(DWORD dwCurTime) {
	if (Effect->dwAni >= 2816 && !bSkill) {
		bSkill = true;
		dwSkillTime = dwCurTime;
	}

	if (GetAsyncKeyState(VK_RIGHT) < 0) { // 오른이동
		if (nXPos + 240 < 1700) {
			nXPos += MOVE_SPEED;
			SetCollision(MOVE_RIGHT);
		}

		if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) {
			nYPos -= MOVE_SPEED;
			SetCollision(MOVE_UP);
		}
		else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
			nYPos += MOVE_SPEED;
			SetCollision(MOVE_DOWN);
		}
	}
	else if (GetAsyncKeyState(VK_LEFT) < 0) { // 왼쪽 이동
		if (nXPos > -85) {
			nXPos -= MOVE_SPEED;
			SetCollision(MOVE_LEFT);
		}

		if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) {
			nYPos -= MOVE_SPEED;
			SetCollision(MOVE_UP);
		}
		else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
			nYPos += MOVE_SPEED;
			SetCollision(MOVE_DOWN);
		}
	}
	else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) { // 아래 이동
		nYPos += MOVE_SPEED;
		SetCollision(MOVE_DOWN);
	}
	else if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) { // 위 이동
		nYPos -= MOVE_SPEED;
		SetCollision(MOVE_UP);
	}

	skillCollision.top = collision.top - 180;
	skillCollision.bottom = collision.top + 170;

	if (bSkill && dwCurTime - dwSkillTime > 2000) {
		dwSkillTime = dwCurTime;

		bSkill = false;
		state = PLAYER_IDLE;
		Effect->dwAni = 0;
	}
}

void CPlayer::Respawn(DWORD dwCurTime) {
	if (dwCurTime - dwRespawnTime >= 1000) {
		dwRespawnTime = dwCurTime;
		dwRespawnCount++;

		if (dwRespawnCount >= 3) {
			dwRespawnCount = 0;
			state = PLAYER_IDLE;
		}
	}

	if (GetAsyncKeyState(VK_SPACE) < 0) { // 공격(최우선)
		if (GetAsyncKeyState(VK_RIGHT) < 0 && nXPos + 240 < 1700) {
			nXPos += MOVE_SPEED;
			SetCollision(MOVE_RIGHT);

			if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) {
				nYPos -= MOVE_SPEED;
				SetCollision(MOVE_UP);
			}
			else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
				nYPos += MOVE_SPEED;
				SetCollision(MOVE_DOWN);
			}
		}
		else if (GetAsyncKeyState(VK_LEFT) < 0 && nXPos > -85) {
			nXPos -= MOVE_SPEED;
			SetCollision(MOVE_LEFT);

			if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) { // 왜 안되는지 모르겠음
				nYPos -= MOVE_SPEED;
				SetCollision(MOVE_UP);
			}
			else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
				nYPos += MOVE_SPEED;
				SetCollision(MOVE_DOWN);
			}
		}
		else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
			nYPos += MOVE_SPEED;
			SetCollision(MOVE_DOWN);
		}
		else if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) {
			nYPos -= MOVE_SPEED;
			SetCollision(MOVE_UP);
		}

		if (dwCurTime - dwWeaponTime >= 50) {
			dwWeaponTime = dwCurTime;
			Attack();
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT) < 0) { // 오른이동
		if (nXPos + 240 < 1700) {
			nXPos += MOVE_SPEED;
			SetCollision(MOVE_RIGHT);
		}

		if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) {
			nYPos -= MOVE_SPEED;
			SetCollision(MOVE_UP);
		}
		else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
			nYPos += MOVE_SPEED;
			SetCollision(MOVE_DOWN);
		}
	}
	else if (GetAsyncKeyState(VK_LEFT) < 0) { // 왼쪽 이동
		if (nXPos > -85) {
			nXPos -= MOVE_SPEED;
			SetCollision(MOVE_LEFT);
		}

		if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) {
			nYPos -= MOVE_SPEED;
			SetCollision(MOVE_UP);
		}
		else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) {
			nYPos += MOVE_SPEED;
			SetCollision(MOVE_DOWN);
		}
	}
	else if (GetAsyncKeyState(VK_DOWN) < 0 && nYPos + 210 < 800) { // 아래 이동
		nYPos += MOVE_SPEED;
		SetCollision(MOVE_DOWN);
	}
	else if (GetAsyncKeyState(VK_UP) < 0 && nYPos > -100) { // 위 이동
		nYPos -= MOVE_SPEED;
		SetCollision(MOVE_UP);
	}
}

void CPlayer::Attack() {
	for (int i = 0; i < 68; i++) {
		if (!Weapon[i].On && !Weapon[i].bCollision) {
			Weapon[i].On = true;
			Weapon[i].SetWeapon(nXPos, nYPos, dwWeaponLV);

			m_pSystem->playSound(m_pAttack, 0, false, &m_pChAttack);
			m_pChAttack->setVolume(0.3f);
			break;
		}
	}
}

void CPlayer::SetCollision(PLAYER_MOVE move) {
	switch (move)
	{
		case MOVE_RIGHT:
		{
			collision.left += MOVE_SPEED;
			collision.right += MOVE_SPEED;
			break;
		}
		case MOVE_LEFT:
		{
			collision.left -= MOVE_SPEED;
			collision.right -= MOVE_SPEED;
			break;
		}
		case MOVE_UP:
		{
			collision.top -= MOVE_SPEED;
			collision.bottom -= MOVE_SPEED;
			break;
		}
		case MOVE_DOWN:
		{
			collision.top += MOVE_SPEED;
			collision.bottom += MOVE_SPEED;
			break;
		}
	}
}

void CPlayer::SetEffect(DWORD dwCurTime) {
	// 부스터 ----------------------------------------------------------------------
	if (state == PLAYER_DIE || state == PLAYER_RESPAWN) {
		Booster->On = false;
		Booster->dwAni = 0;
	}
	else {
		Booster->On = true;

		Booster->nXPos = nXPos - 40;
		Booster->nYPos = nYPos;

		if (dwCurTime - Booster->dwAniTime > 70) {
			Booster->dwAniTime = dwCurTime;
			if (Booster->dwAni >= 1280) {
				Booster->dwAni = 0;
			}
			else {
				Booster->dwAni += 256;
			}
		}
	}

	//공격, 스킬 이펙트 -------------------------------------------------------------
	if (state == PLAYER_ATTACK || state == PLAYER_SKILL) {
		Effect->On = true;

		switch (state)
		{
			case PLAYER_ATTACK:
			{
				Effect->state = EFFECT_ATTACK;
				Effect->nXPos = nXPos + 172;
				Effect->nYPos = nYPos + 40;

				if (dwCurTime - Effect->dwAniTime > 100) {
					Effect->dwAniTime = dwCurTime;

					if (Effect->dwAni >= 1792) { // 출력 이펙트
						Effect->dwAni = 0;
					}
					else {
						Effect->dwAni += 256;
					}
				}
				break;
			}
			case PLAYER_SKILL:
			{
				Effect->state = EFFECT_SKILL;
				Effect->nXPos = nXPos + 25;
				Effect->nYPos = nYPos + 6;

				if (dwCurTime - Effect->dwAniTime > 50) {
					Effect->dwAniTime = dwCurTime;

					if (Effect->dwAni < 2816) {
						Effect->dwAni += 256;
					}
				}
				break;
			}
		}
		
	}
	else {
		Effect->On = false;
		Effect->dwAni = 0;
	}

	// 히트(타격) 이펙트 -------------------------------------------------
	for (int i = 0; i < 68; i++) {
		if (Weapon[i].bCollision && dwCurTime - Weapon[i].dwAniTime > 30) {
			Weapon[i].dwAniTime = dwCurTime;
			
			if (Weapon[i].dwAni >= 1344) {
				Weapon[i].dwAni = 0;
				Weapon[i].bCollision = false;
			}
			else {
				Weapon[i].dwAni += 192;
			}
		}
	}
}

void CPlayer::SetAni() {
	switch (state)
	{
		case PLAYER_DIE: {
			if (dwAni < P_ANI_DIE) {
				dwAni += 256;
			}
			else {
				dwAni -= 256;
			}
			break;
		}
		case PLAYER_RESPAWN: {
			if (dwAni <= P_ANI_RESPAWN) {
				dwAni += 256;
			}
			else {
				dwAni = 0;
			}
			break;
		}	
		default: {
			if (dwAni == 0) {
				dwAni = 256;
			}
			else {
				dwAni = 0;
			}
		}
	}
}