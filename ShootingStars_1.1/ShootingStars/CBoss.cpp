#include"CBoss.h"
#define PI 3.14159265358979323846

CBoss::CBoss() {
	bOn = false;
	bCollision = false;

	fTheta = 0.0f;
	State = TYPE_0;
	Pattern = TYPE_0;
	nHP = 4000;
	nStep = 0;

	fXPos = 1700.0f;
	fYPos = 200.0f;

	collision.left = fXPos + 128;
	collision.top = fYPos + 140;
	collision.right = collision.left + 28;
	collision.bottom = collision.top + 100;

	dwMoveTime = 0;
	dwAni = 0;
	dwAniTime = 0;
	dwPatternTime = 0;
}

CBoss::~CBoss() {

}

void CBoss::Update(DWORD Curtime, int PlayerXPos, int PlayerYPos) {
	if (bOn) {
		Move(Curtime);
		ObjectSet(Curtime, PlayerXPos, PlayerYPos);
		HPCheck();
		SetPattern();
	}

	ObjectMove(Curtime);
	SetAni(Curtime);
}

void CBoss::Move(DWORD Curtime) {
	switch (State) {
		case TYPE_0: {
			if (Curtime - dwMoveTime > 20) {
				dwMoveTime = Curtime;

				if (fXPos > 1300.0f) {
					fXPos -= 2.0f;
				}
				else if (fXPos < 1300.0f) {
					fXPos += 2.0f;
				}

				if (fYPos > 200) {
					fYPos -= 1.0f;
				}
				else if (fYPos < 200) {
					fYPos += 1.0f;
				}

				if (fYPos > 200.0f && fYPos < 201.0f) {
					fYPos = 200.0f;
				}

				if (fXPos < 1303.f && fXPos > 1300.0f) {
					fXPos = 1300.0f;
				}
			}
			break;
		}
		case TYPE_A: {
			if (Curtime - dwMoveTime > 40) {
				dwMoveTime = Curtime;
				fXPos += cos(fTheta) * 20;
				fYPos += sin(fTheta) * 20;
				fTheta += 0.25f;
			}
			break;
		}
		case TYPE_B: {
			if (Curtime - dwMoveTime > 30) {
				dwMoveTime = Curtime;
				fYPos += sin(fTheta) * 20;
				fTheta += 0.1f;
			}
			break;
		}
		case TYPE_C: {
			if (Curtime - dwMoveTime > 1000) {
				dwMoveTime = Curtime;
			
				srand((unsigned)time(NULL));
				int nRandom = rand() % 3;

				switch (nRandom) {
					case 0: {
						fXPos = 1400.0f;
						fYPos = 400.0f;
						break;
					}
					case 1: {
						fXPos = 1345.0f;
						fYPos = 10.0f;
						break;
					}
					case 2: {
						fXPos = 1460.0f;
						fYPos = 130.0f;
						break;
					}
				
				}
			}
			break;
		}
		case TYPE_D: {
			if (Curtime - dwMoveTime > 80) {
				dwMoveTime = Curtime;
				
				fXPos -= 0.5f;
			}
			break;
		}
	}

	collision.left = fXPos + 128;
	collision.top = fYPos + 140;
	collision.right = collision.left + 28;
	collision.bottom = collision.top + 100;
}

void CBoss::SetAni(DWORD Curtime) {
	if (Curtime - dwAniTime > 100) {
		dwAniTime = Curtime;

		if (bCollision) {
			dwAni += 150;
			if (dwAni >= 600) {
				bCollision = false;
				dwAni = 0;
			}
		}
		else if (bOn) {
			dwAni += 256;
			if (dwAni >= 2304) {
				dwAni = 0;
			}
		}
	}
}

void CBoss::ObjectMove(DWORD Curtime) {
	for (int i = 0; i < 300; i++) {
		switch (Pattern) {
			case TYPE_0: {
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 30) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos += 40.0f;
				}
				break;
			}
			case TYPE_A: {
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 10) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos += cos(Object[i].fTheta) * 6;
					Object[i].fYPos += sin(Object[i].fTheta) * 6;
				}
				break;
			}
			case TYPE_B: {
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 20) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos -= 30.0f;
				}
				break;
			}
			case TYPE_C: {
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 15) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos += cos(Object[i].fTheta) * 20;
					Object[i].fYPos += sin(Object[i].fTheta) * 20;
				}
				break;
			}
			case TYPE_D: {
				if (Object[i].bOn == true && Curtime - Object[i].dwTime > 20) {
					Object[i].dwTime = Curtime;

					Object[i].fXPos += cos(Object[i].fTheta) * 26;
					Object[i].fYPos += sin(Object[i].fTheta) * 26;
				}
				break;
			}
		}

		Object[i].collision.left = Object[i].fXPos;
		Object[i].collision.right = Object[i].fXPos + 30;
		Object[i].collision.top = Object[i].fYPos;
		Object[i].collision.bottom = Object[i].fYPos + 30;

		if (Object[i].fXPos >= 1700 || Object[i].fXPos <= -30 || Object[i].fYPos >= 800 || Object[i].fYPos <= -30) {
			Object[i].bOn = false;
		}
	}
}

void CBoss::ObjectSet(DWORD Curtime, int PlayerXPos, int PlayerYPos) {
	switch (State)
	{
		case TYPE_A: {
			if (Curtime - dwPatternTime > 500) {
				dwPatternTime = Curtime;
				float theta = 2.7f;
				WORD count = 0;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						Object[i].bOn = true;
						Object[i].fXPos = collision.left;
						Object[i].fYPos = collision.top - 10;
						Object[i].fTheta = theta;
					
						if (i % 2 == 0) {
							theta += 0.2f;
						}
						else {
							theta += 0.3f;
						}

						count++;
						if (count > 5) break;
					}
				}
			}
			break;
		}
		case TYPE_B: {
			if (Curtime - dwPatternTime > 50) {
				dwPatternTime = Curtime;
				float count = -100.0f;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						Object[i].bOn = true;
						Object[i].fXPos = collision.left;
						Object[i].fYPos = collision.top - 10 + count;

						if (count >= 200.0f) {
							break;
						}
						count += 300.0f;
					}
				}
			}
			break;
		}
		case TYPE_C: {
			if (Curtime - dwPatternTime > 150) {
				dwPatternTime = Curtime;

				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						float h = (float)PlayerYPos - collision.top - 10;
						float w = (float)PlayerXPos - collision.left;
						float fAngle = atan2(h, w);// *180 / PI;

						Object[i].bOn = true;
						Object[i].fXPos = collision.left;
						Object[i].fYPos = collision.top - 10;

						Object[i].fTheta = fAngle;
						break;
					}
				}
			}
			break;
		}
		case TYPE_D: {
			if (Curtime - dwPatternTime > 30) {
				dwPatternTime = Curtime;

				int nCount = 0;
				float fSubTheta = fTheta;
				fTheta += 0.2f;
				for (int i = 0; i < 300; i++) {
					if (Object[i].bOn == false) {
						Object[i].fTheta = fSubTheta;
						fSubTheta += 1.45f;
						
						Object[i].bOn = true;
						Object[i].fXPos = collision.left;
						Object[i].fYPos = collision.top - 10;

						nCount++;
						if (nCount == 4) {
							break;
						}
					}
				}
			}
			break;
		}
	}
}

void CBoss::HPCheck() {
	if (nHP <= 0) {
		bOn = false;
		bCollision = true;
		dwAni = 0;
	}
}

void CBoss::SetPattern() {
	switch (nStep) {
		case 0: {
			State = TYPE_0;
			Pattern = TYPE_0;
			if (fXPos == 1300.0f && fYPos == 200.0f) {
				State = TYPE_A;
				Pattern = TYPE_A;
				nStep++;
			}
			break;
		}
		case 1: {
			if (nHP <= 3000) {
				State = TYPE_0;
				nStep++;
			}
			break;
		}
		case 2: {
			bool bCheck = true;
			for (int i = 0; i < 300; i++) {
				if (Object[i].bOn) {
					bCheck = false;
				}
			}
			
			if (bCheck && fXPos == 1300.0f && fYPos == 200.0f) {
				Pattern = TYPE_B;
				State = TYPE_B;
				nStep++;
			}
			break;
		}
		case 3: {
			if (nHP <= 2000) {
				State = TYPE_0;
				nStep++;
			}
			break;
		}
		case 4: {
			bool bCheck = true;
			for (int i = 0; i < 300; i++) {
				if (Object[i].bOn) {
					bCheck = false;
				}
			}

			if (bCheck && fXPos == 1300.0f && fYPos == 200.0f) {
				Pattern = TYPE_C;
				State = TYPE_C;
				nStep++;
			}
			break;
		}
		case 5: {
			if (nHP <= 1000) {
				State = TYPE_0;
				nStep++;
			}
			break;
		}
		case 6: {
			bool bCheck = true;
			for (int i = 0; i < 300; i++) {
				if (Object[i].bOn) {
					bCheck = false;
				}
			}

			if (bCheck && fXPos == 1300.0f && fYPos == 200.0f) {
				Pattern = TYPE_D;
				State = TYPE_D;
				nStep++;
			}
			break;
		}
		case 7: {
			Pattern = TYPE_D;
			break;
		}
	}
}