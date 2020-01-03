#include"CBackground.h"

CBackground::CBackground() {
	dwBackgroundTime = 0;
	dwObjectTime = 0;

	dwBackgroundXpos = 0;
	dwObjectXPos = 0;
	dwObjectYPos = 500;
}

CBackground::~CBackground() {
	
}

void CBackground::Update(DWORD CurTime) {
	if (CurTime - dwBackgroundTime > 50) { // 배경
		dwBackgroundTime = CurTime;

		dwBackgroundXpos += 1;

		if (dwBackgroundXpos >= 2500) {
			dwBackgroundXpos = 0;
		}
	}

	if (CurTime - dwObjectTime > 30) {  // 오브젝트
		dwObjectTime = CurTime;
		if (dwObjectXPos + 40 < 1700) {
			dwObjectXPos += 40;
		}
		else
		{
			dwObjectXPos = 0;
		}
	}
}