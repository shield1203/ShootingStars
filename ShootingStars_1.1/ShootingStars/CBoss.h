#pragma once
#include<Windows.h>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include"Define.h"
#include"CEnemyObject.h"

class CBoss {
public:
	bool bOn;
	bool bCollision;

	BOSS_PATTERN State;
	BOSS_PATTERN Pattern;

	int nHP;
	int nStep;

	float fXPos;
	float fYPos;
	float fTheta;

	RECT collision;

	CEnemyObject Object[300];

	DWORD dwMoveTime;
	DWORD dwPatternTime;
	DWORD dwAni;
	DWORD dwAniTime;

	void Update(DWORD Curtime, int PlayerXPos, int PlayerYPos);
	void Move(DWORD Curtime);
	void SetAni(DWORD Curtime);
	void ObjectMove(DWORD Curtime);
	void ObjectSet(DWORD Curtime, int PlayerXPos, int PlayerYPos);
	void HPCheck();
	void SetPattern();

	CBoss();
	~CBoss();
};