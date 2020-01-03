#pragma once
#include<Windows.h>
#include"Define.h"

class CBackground {
public:
	DWORD dwBackgroundTime;
	DWORD dwObjectTime;

	DWORD dwBackgroundXpos;
	DWORD dwObjectXPos;
	DWORD dwObjectYPos;

	void Update(DWORD CurTime);

	CBackground();
	~CBackground();
};