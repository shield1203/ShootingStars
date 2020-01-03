#pragma once
#include<Windows.h>
#include"Define.h"

class CEffect {
public:
	bool On;
	EFFECT_STATE state;

	DWORD dwAniTime;
	DWORD dwAni;

	int nXPos; // 음수값이 필요함
	int nYPos;

	CEffect(EFFECT_STATE State);
	~CEffect();
};