#pragma once
#include<Windows.h>

class CEnemyObject {
public:
	bool bOn = false;
	float fXPos;
	float fYPos;
	float fTheta = 0.0f;

	RECT collision;

	DWORD dwTime = 0;
};