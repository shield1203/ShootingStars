#pragma once
#include<Windows.h>
#include"Define.h"

class CEffect {
public:
	bool On;
	EFFECT_STATE state;

	DWORD dwAniTime;
	DWORD dwAni;

	int nXPos; // �������� �ʿ���
	int nYPos;

	CEffect(EFFECT_STATE State);
	~CEffect();
};