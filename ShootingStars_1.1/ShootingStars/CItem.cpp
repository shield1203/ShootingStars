#include"CItem.h"

CItem::CItem() {
	bOn = false;
	dwAni = 0;
	dwAniTime = 0;
	dwMoveTime = 0;
}

CItem::~CItem() {

}

void CItem::Update(DWORD dwCurTime) {
	if (bOn) {
		Move(dwCurTime);
		SetAni(dwCurTime);
	}
}

void CItem::Init(ITEM_KIND kind, float fXPos, float fYPos) {
	bOn = true;
	this->kind = kind;
	
	this->nXPos = fXPos;
	this->nYPos = fYPos;

	collision.left = nXPos;
	collision.top = nYPos;
	collision.right = nXPos + 60;
	collision.bottom = nYPos + 60;
}

void CItem::Move(DWORD dwCurTime) {
	if (dwCurTime - dwMoveTime > 20) {
		dwMoveTime = dwCurTime;
		nXPos -= 7;
		collision.left -= 7;
		collision.right -= 7;

		if (collision.right <= 0) {
			bOn = false;
		}
	}
}

void CItem::SetAni(DWORD dwCurTime) {
	if (dwCurTime - dwAniTime > 500) {
		dwAniTime = dwCurTime;
		if (dwAni >= 90) {
			dwAni = 0;
		}
		else {
			dwAni += 30;
		}
	}
}