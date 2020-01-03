#pragma once
#include<Windows.h>
#include"Define.h"

class CItem {
public:
	ITEM_KIND kind;

	bool bOn = false;

	DWORD dwAniTime;
	DWORD dwAni;

	DWORD dwMoveTime;

	int nXPos; 
	int nYPos;

	RECT collision; // Ãæµ¹°ª

	void Update(DWORD dwCurTime);
	void Init(ITEM_KIND kind, float fXPos, float fYPos);
	void Move(DWORD dwCurTime);
	void SetAni(DWORD dwCurTime);

	CItem();
	~CItem();
};