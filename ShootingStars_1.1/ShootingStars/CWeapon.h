#pragma once
#include<Windows.h>
#include"Define.h"
#include"CEffect.h"

class CWeapon {
public:
	bool On;
	bool bCollision;

	int nXPos;
	int nYPos;

	RECT rtLV[3]; // 레벨별 범위
	RECT area;

	DWORD dwAni;
	DWORD dwAniTime;
	DWORD dwMoveTime;

	void SetWeapon(int nXPos, int nYPos, DWORD dwWeaponLV);
	void Move(DWORD CurTime);

	CWeapon();
	~CWeapon();
};