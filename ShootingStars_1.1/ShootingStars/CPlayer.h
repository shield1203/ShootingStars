#pragma once
#include<Windows.h>
#include"Define.h"
#include"CEffect.h"
#include"CWeapon.h"
#include"inc/fmod.hpp"

#pragma comment(lib, "fmod_vc.lib")

class CPlayer {
public:
	PLAYER_STATE prevState;
	PLAYER_STATE state;

	DWORD dwLifeCount;
	DWORD dwSkillCount;
	DWORD dwWeaponLV;
	DWORD dwWeaponTime;
	DWORD dwDieTime;
	DWORD dwRespawnTime;
	DWORD dwRespawnCount;
	DWORD dwSkillTime;

	DWORD dwAniTime;
	DWORD dwAni;

	DWORD dwScore;

	int nXPos; // 음수값이 필요함
	int nYPos;

	bool bSkill;

	CWeapon Weapon[68];
	RECT WeaponLV[3];

	RECT collision; // 충돌값
	RECT skillCollision;

	CEffect *Booster;
	CEffect *Effect;

	FMOD::System* m_pSystem;
	FMOD::Sound* m_pSkill, * m_pSkillEffect, * m_pAttack, * m_pRespawn;
	FMOD::Channel* m_pChannel, *m_pChAttack, *m_pChSkill;

	void Update(DWORD dwCurTime);
	void Action(DWORD dwCurTime);
	void Die(DWORD dwCurTime);
	void Skill(DWORD dwCurTime);
	void Respawn(DWORD dwCurTime);
	void Attack();
	void SetEffect(DWORD dwCurTime);
	void SetCollision(PLAYER_MOVE move);
	void SetAni();

	CPlayer();
	~CPlayer();
};