#pragma once
#include<Windows.h>
#include"Define.h"

struct CStage {
	int nHP, nXPos, nYPos;
	DWORD dwTime;
	ENEMY_TYPE EnemyType;
	ENEMY_MOVE MovePattern;
	ENEMY_PATTERN AttackPattern;
};