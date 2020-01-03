#pragma once
#include<Windows.h>
#include<cmath>
#include"Define.h"
#include"CEnemyObject.h"

class CEnemy {
public:
	bool bOn;
	bool bCollision;

	ENEMY_TYPE Type;
	ENEMY_PATTERN Pattern;
	ENEMY_MOVE MovePattern;

	int nHP;

	float fXPos;
	float fYPos;
	float fTheta;

	RECT collision;

	CEnemyObject Object[300];

	DWORD dwMoveTime;
	DWORD dwPatternTime;
	DWORD dwPatternSubTime;
	DWORD dwObjectCount;
	DWORD dwAni;
	DWORD dwAniTime;

	void Init(ENEMY_TYPE Type, ENEMY_PATTERN Pattern, ENEMY_MOVE MovePattern, int nHP, float fXPos, float fYPos);
	void Update(DWORD Curtime, int PlayerXPos, int PlayerYPos);
	void Move(DWORD Curtime);
	void SetAni(DWORD Curtime);
	void ObjectMove(DWORD Curtime);
	void ObjectSet(DWORD Curtime, int PlayerXPos, int PlayerYPos);
	void HPCheck();

	CEnemy();
	~CEnemy();
};

