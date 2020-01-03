#include"CEnemy.h"
#define PI 3.14159265358979323846

CEnemy::CEnemy() {
	bOn = true;
	bCollision = false;

	fTheta = 0.0f;
	
	collision.left = fXPos;
	collision.top = fYPos;
	collision.right = fXPos + 30;
	collision.bottom = fXPos + 100;

	dwMoveTime = 0;
	dwAni = 0;
	dwAniTime = 0;
	dwPatternSubTime = 0;

	dwObjectCount = 0;
}

CEnemy::~CEnemy() {
	
}

void CEnemy::Init(ENEMY_TYPE Type, ENEMY_PATTERN Pattern, ENEMY_MOVE MovePattern, int nHP, float fXPos, float fYPos) {
	this->Type = Type;
	this->Pattern = Pattern;
	this->MovePattern = MovePattern; 

	this->nHP = nHP;

	this->fXPos = fXPos;
	this->fYPos = fYPos;

	bOn = true;
}

void CEnemy::Update(DWORD Curtime, int PlayerXPos, int PlayerYPos) {
	if (bOn) {
		Move(Curtime);
		ObjectSet(Curtime, PlayerXPos, PlayerYPos);
		HPCheck();
	}
	
	ObjectMove(Curtime);
	SetAni(Curtime);
}

void CEnemy::Move(DWORD Curtime) {
	switch (MovePattern) {
		case MOVE_R_L: {
			if (Curtime - dwMoveTime > 20) {
				dwMoveTime = Curtime;
				fXPos -= 5.0f;

				if (fXPos < -100) {
					bOn = false;
				}
			}
			break;
		}
		case MOVE_R_L_FS: {
			if (Curtime - dwMoveTime > 20) {
				dwMoveTime = Curtime;
				if (fXPos > 1500) {
					fXPos -= 20.0f;
				}
				else {
					fXPos -= 2.0f;
				}

				if (fXPos < -100) {
					bOn = false;
				}
			}
			break;
		}
		case MOVE_R_L_ST: {
			if (Curtime - dwMoveTime > 20) {
				dwMoveTime = Curtime;
				if (fXPos > 1400) {
					fXPos -= 10.0f;
				}
			}
			break;
		}
		case MOVE_U_D: {
			if (Curtime - dwMoveTime > 20) {
				dwMoveTime = Curtime;
				fYPos += 7.0f;

				if (fYPos > 800) {
					bOn = false;
				}
			}
			break;
		}
		case MOVE_U_D_FS: {
			if (Curtime - dwMoveTime > 20) {
				dwMoveTime = Curtime;
				if (fYPos < 170) {
					fYPos += 20.0f;
				}
				else {
					fYPos += 2.0f;
				}

				if (fYPos > 800) {
					bOn = false;
				}
			}
			break;
		} 
		case MOVE_U_D_ST: {
			if (Curtime - dwMoveTime > 20) {
				dwMoveTime = Curtime;
				if (fYPos < 300) {
					fYPos += 15.0f;
				}
			}
			break;
		}
		case MOVE_D_U: {
			if (Curtime - dwMoveTime > 20) {
				dwMoveTime = Curtime;
				fYPos -= 7.0f;

				if (fYPos < -100) {
					bOn = false;
				}
			}
			break;
		}
		case MOVE_D_U_FS: {
			if (Curtime - dwMoveTime > 20) {
				dwMoveTime = Curtime;

				if (fYPos > 630) {
					fYPos -= 20.0f;
				}
				else {
					fYPos -= 2.0f;
				}

				if (fYPos < -100) {
					bOn = false;
				}
			}
			break;
		}
		case MOVE_D_U_ST: {
			if (Curtime - dwMoveTime > 20) {
				dwMoveTime = Curtime;
				if (fYPos > 500) {
					fYPos -= 20.0f;
				}
			}
			break;
		}
		case MOVE_RU_LD: {
			if (Curtime - dwMoveTime > 30) {
				dwMoveTime = Curtime;
				fXPos -= 11.0f;
				fYPos += 4.0f;

				if (fYPos > 800 || fXPos < -100) {
					bOn = false;
				}
			}
			break;
		}
		case MOVE_C_R_L: {
			if (Curtime - dwMoveTime > 30) {
				dwMoveTime = Curtime;
				fXPos -= 1.0f;
				fYPos += sin(fTheta) * 10;
				fTheta += 0.1f;

				if (fXPos < -100) {
					bOn = false;
				}
			}
			break;
		}
	}

	collision.left = fXPos;
	collision.right = fXPos + 30;
	collision.top = fYPos;
	collision.bottom = fYPos + 100;
}

void CEnemy::SetAni(DWORD Curtime) {
	switch (Type) {
		case NOMAL_NECO:
		{
			if (Curtime - dwAniTime > 100) {
				dwAniTime = Curtime;

				if (bCollision) {
					dwAni += 100;
					if (dwAni > 900) {
						bCollision = false;
						dwAni = 0;
					}
				}
				else if (bOn) {
					dwAni += 50;
					if (dwAni >= 250) {
						dwAni = 0;
					}
				}
			}
			break;
		}
		case GRAY_NECO:
		{
			if (Curtime - dwAniTime > 115) {
				dwAniTime = Curtime;

				if (bCollision) {
					dwAni += 100;
					if (dwAni > 900) {
						bCollision = false;
						dwAni = 0;
					}
				}
				else if (bOn) {
					dwAni += 50;
					if (dwAni >= 350) {
						dwAni = 0;
					}
				}
			}
		}
		case MECA_GIRL:
		{
			if (Curtime - dwAniTime > 110) {
				dwAniTime = Curtime;

				if (bCollision) {
					dwAni += 100;
					if (dwAni > 900) {
						bCollision = false;
						dwAni = 0;
					}
				}
				else if (bOn) {
					dwAni += 150;
					if (dwAni >= 300) {
						dwAni = 0;
					}
				}
			}
		}
	}
	
}

void CEnemy::ObjectMove(DWORD Curtime) {
	for (int i = 0; i < 300; i++) {
		switch (Pattern) {
			case TYPE_1: { 
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 20) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos += cos(Object[i].fTheta) * 20;
					Object[i].fYPos += sin(Object[i].fTheta) * 20;

					Object[i].collision.left = Object[i].fXPos;
					Object[i].collision.right = Object[i].fXPos + 20;
					Object[i].collision.top = Object[i].fYPos;
					Object[i].collision.bottom = Object[i].fYPos + 20;
				}
				break;
			}
			case TYPE_2: { 
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 28) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos += cos(Object[i].fTheta) * 8;
					Object[i].fYPos += sin(Object[i].fTheta) * 8;

					Object[i].collision.left = Object[i].fXPos;
					Object[i].collision.right = Object[i].fXPos + 20;
					Object[i].collision.top = Object[i].fYPos;
					Object[i].collision.bottom = Object[i].fYPos + 20;
				}
				break;
			}
			case TYPE_3: {
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 20) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos += cos(Object[i].fTheta) * 10;
					Object[i].fYPos += sin(Object[i].fTheta) * 10;

					Object[i].collision.left = Object[i].fXPos;
					Object[i].collision.right = Object[i].fXPos + 20;
					Object[i].collision.top = Object[i].fYPos;
					Object[i].collision.bottom = Object[i].fYPos + 20;
				}
				break;
			}
			case TYPE_4: {
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 25) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos += cos(Object[i].fTheta) * 10;
					Object[i].fYPos += sin(Object[i].fTheta) * 10;

					Object[i].collision.left = Object[i].fXPos;
					Object[i].collision.right = Object[i].fXPos + 20;
					Object[i].collision.top = Object[i].fYPos;
					Object[i].collision.bottom = Object[i].fYPos + 20;
				}
				break;
			}
			case TYPE_5: {
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 20) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos += cos(Object[i].fTheta) * 10;
					Object[i].fYPos += sin(Object[i].fTheta) * 10;

					Object[i].collision.left = Object[i].fXPos;
					Object[i].collision.right = Object[i].fXPos + 20;
					Object[i].collision.top = Object[i].fYPos;
					Object[i].collision.bottom = Object[i].fYPos + 20;
				}
				break;
			}
			case TYPE_6: { // 왼쪽으로만 이동
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 30) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos -= 20.0f;

					Object[i].collision.left = Object[i].fXPos;
					Object[i].collision.right = Object[i].fXPos + 20;
					Object[i].collision.top = Object[i].fYPos;
					Object[i].collision.bottom = Object[i].fYPos + 20;
				}
				break;
			}
			case TYPE_7: {
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 25) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos += cos(Object[i].fTheta) * 10;
					Object[i].fYPos += sin(Object[i].fTheta) * 10;

					Object[i].collision.left = Object[i].fXPos;
					Object[i].collision.right = Object[i].fXPos + 20;
					Object[i].collision.top = Object[i].fYPos;
					Object[i].collision.bottom = Object[i].fYPos + 20;
				}
				break;
			}
			case TYPE_8: { // 왼쪽으로만 이동
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 20) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos -= 30.0f;

					Object[i].collision.left = Object[i].fXPos;
					Object[i].collision.right = Object[i].fXPos + 20;
					Object[i].collision.top = Object[i].fYPos;
					Object[i].collision.bottom = Object[i].fYPos + 20;
				}
				break;
			}
			case TYPE_9: { // 탄 자체세타에 따라 계속 바뀐다
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 20) {
					Object[i].dwTime = Curtime;

					Object[i].fTheta += 0.2f;

					if (Object[i].fTheta >= 50.0f) {
						Object[i].fTheta = 0;
					}

					Object[i].fXPos -= 10.0f;
					Object[i].fYPos += sin(Object[i].fTheta) * 10;

					Object[i].collision.left = Object[i].fXPos;
					Object[i].collision.right = Object[i].fXPos + 20;
					Object[i].collision.top = Object[i].fYPos;
					Object[i].collision.bottom = Object[i].fYPos + 20;
				}
				break;
			}
			
		}

		if (Object[i].fXPos > 1700 || Object[i].fXPos < -20 || Object[i].fYPos > 800 || Object[i].fYPos < -20) {
			Object[i].bOn = false;
			Object[i].fTheta = 0;
		}
	}
}

void CEnemy::ObjectSet(DWORD Curtime, int PlayerXPos, int PlayerYPos) {
	switch (Pattern)
	{
		case TYPE_1: { // 부채꼴 5발 연속 발사느림 이동빠름
			if (Curtime - dwPatternTime > 1500) {
				dwPatternTime = Curtime;
				float theta = 2.5f;
				WORD count = 0;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						Object[i].bOn = true;
						Object[i].fXPos = fXPos;
						Object[i].fYPos = fYPos;
						Object[i].fTheta = theta;
						theta += 0.2f;
						count++;

						if (count > 4) break;
					}
				}
			}
			break;
		}
		case TYPE_2: { // 부채꼴 3발 연속 발사중간 이동중간
			if (Curtime - dwPatternTime > 1000) {
				dwPatternTime = Curtime;
				float theta = 3.0f;
				WORD count = 0;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						Object[i].bOn = true;
						Object[i].fXPos = fXPos;
						Object[i].fYPos = fYPos;
						Object[i].fTheta = theta;
						theta += 0.1f;
						count++;

						if (count > 3) break;
					}
				}
			}
			break;
		}
		case TYPE_3: { // 부채꼴 3발 3줄 발사중간 이동느림
			if (Curtime - dwPatternSubTime > 2500) {
				dwPatternSubTime = Curtime;
				dwObjectCount = 3;
			}

			if (Curtime - dwPatternTime > 300 && dwObjectCount > 0) {
				dwPatternTime = Curtime;
				float theta = 3.0f;
				WORD count = 0;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						Object[i].bOn = true;
						Object[i].fXPos = fXPos;
						Object[i].fYPos = fYPos;
						Object[i].fTheta = theta;
						theta += 0.1f;
						count++;

						if (count > 3) break;
					}
				}
				dwObjectCount--;
			}
			break;
		}
		case TYPE_4: { // 유도 연속 발사 느림 이동 빠름
			if (Curtime - dwPatternTime > 1400) {
				dwPatternTime = Curtime;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						float h = (float)PlayerYPos - fYPos;
						float w = (float)PlayerXPos - (fXPos + 50);
						float fAngle = atan2(h, w);// *180 / PI;

						Object[i].bOn = true;
						Object[i].fXPos = fXPos;
						Object[i].fYPos = fYPos + 50;

						Object[i].fTheta = fAngle;

						break;
					}
				}
			}
			break;
		}
		case TYPE_5: { // 유도 3발씩 빠름 느림
			if (Curtime - dwPatternSubTime > 1500) {
				dwPatternSubTime = Curtime;
				dwObjectCount = 3;
			}

			if (Curtime - dwPatternTime > 250 && dwObjectCount > 0) {
				dwPatternTime = Curtime;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						float h = (float)PlayerYPos - fYPos;
						float w = (float)PlayerXPos - fXPos;
						float fAngle = atan2(h, w);// *180 / PI;

						Object[i].bOn = true;
						Object[i].fXPos = fXPos;
						Object[i].fYPos = fYPos;
						Object[i].fTheta = fAngle;
						break;
					}
				}
				dwObjectCount--;
			}
			break;
		}
		case TYPE_6: { // 물결 2발 연속 빠름 빠름
			if (Curtime - dwPatternTime > 50) {
				dwPatternTime = Curtime;
				float count = -50.0f;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						Object[i].bOn = true;
						Object[i].fXPos = fXPos;
						Object[i].fYPos = fYPos + count;

						if (count >= 150.0f) {
							break;
						}
						count += 230.0f;
					}
				}
			}
			break;
		}
		case TYPE_7: { // 돌려서
			if (Curtime - dwPatternTime > 50) {
				dwPatternTime = Curtime;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						fTheta += 0.15f;
						Object[i].fTheta = fTheta;
						Object[i].bOn = true;
						Object[i].fXPos = fXPos;
						Object[i].fYPos = fYPos + 35;

						break;
					}
				}
			}
			break;
		}
		case TYPE_8: { // 직선 2발 3줄 발사빠름 이동빠름
			if (Curtime - dwPatternSubTime > 1500) {
				dwPatternSubTime = Curtime;
				dwObjectCount = 3;
			}

			if (Curtime - dwPatternTime > 250 && dwObjectCount > 0) {
				dwPatternTime = Curtime;
				WORD count = 1;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						Object[i].bOn = true;
						Object[i].fXPos = fXPos;
						Object[i].fYPos = fYPos + (25 * count);
						count++;

						if (count > 2) break;
					}
				}
				dwObjectCount--;
			}
			break;
		}
		case TYPE_9: { // 탄이 각도에 따라서 계속 방향을 바꿈
			if (Curtime - dwPatternSubTime > 2000) {
				dwPatternSubTime = Curtime;
				dwObjectCount = 5;
			}

			if (Curtime - dwPatternTime > 250 && dwObjectCount > 0) {
				dwPatternTime = Curtime;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						Object[i].bOn = true;
						Object[i].fXPos = fXPos;
						Object[i].fYPos = fYPos + 30;
						break;
					}
				}
				dwObjectCount--;
			}
			break;
		}
	}
}

void CEnemy::HPCheck() {
	if (nHP <= 0) {
		bOn = false;
		bCollision = true;
		dwAni = 0;
	}
}