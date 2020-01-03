#include"CWeapon.h"

CWeapon::CWeapon() {
	On = false;
	bCollision = false;

	nXPos = 0;
	nYPos = 0;

	rtLV[0].left = 0; // ºÒ¸´ Ã³À½ ¹üÀ§
	rtLV[0].right = 25; // ºÒ¸´ ³¡ X °¡·Î
	rtLV[0].top = 11; // ºÒ¸´ ½ÃÀÛ
	rtLV[0].bottom = 19; // ºÒ¸´³¡ Y ¼¼·Î

	rtLV[1].left = 0;
	rtLV[1].right = 35;
	rtLV[1].top = 6;
	rtLV[1].bottom = 23;

	rtLV[2].left = 0;
	rtLV[2].right = 35;
	rtLV[2].top = 2;
	rtLV[2].bottom = 28;

	dwAni = 0;
	dwAniTime = 0;
	dwMoveTime = 0;
}

CWeapon::~CWeapon() {
	
}

void CWeapon::SetWeapon(int nXPos, int nYPos, DWORD dwWeaponLV) {
	area.left = nXPos + 190 + rtLV[dwWeaponLV].left;
	area.right = nXPos + 190 + rtLV[dwWeaponLV].right;
	area.top = nYPos + 155 + rtLV[dwWeaponLV].top;
	area.bottom = nYPos+ 155 + rtLV[dwWeaponLV].bottom;

	this->nXPos = nXPos + 190;// area.left;
	this->nYPos = nYPos + 155;// area.top;
}

void CWeapon::Move(DWORD CurTime) {
	if (CurTime - dwMoveTime >= 30) {
		dwMoveTime = CurTime;

		nXPos += 30;
		area.left += 30;
		area.right += 30;
	}

	if (nXPos >= 1700) {
		On = false;
	}
}