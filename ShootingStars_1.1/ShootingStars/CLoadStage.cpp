#include"CLoadStage.h"

void CLoadStage::LoadFile(vector<CStage*>& StageInfo, const char* strFileName) {
	TiXmlDocument doc;
	doc.LoadFile(strFileName);

	//루트노드 접근
	TiXmlElement* pRoot = doc.FirstChildElement("Stage");
	if (!pRoot) return;

	TiXmlElement* pElem = pRoot->FirstChildElement("Enemy");
	TiXmlAttribute* pAttrib = nullptr;

	string str;
	while (pElem) {
		pAttrib = pElem->FirstAttribute();

		CStage* addEnemy = new CStage();

		addEnemy->dwTime = atoi(pAttrib->Value()); // 시간
		pAttrib = pAttrib->Next();

		str = pAttrib->Value(); // 공격 패턴---------------------
		if (str == "TYPE_1") {
			addEnemy->AttackPattern = TYPE_1;
		}
		else if (str == "TYPE_2") {
			addEnemy->AttackPattern = TYPE_2;
		}
		else if (str == "TYPE_3") {
			addEnemy->AttackPattern = TYPE_3;
		}
		else if (str == "TYPE_4") {
			addEnemy->AttackPattern = TYPE_4;
		}
		else if (str == "TYPE_5") {
			addEnemy->AttackPattern = TYPE_5;
		}
		else if (str == "TYPE_6") {
			addEnemy->AttackPattern = TYPE_6;
		}
		else if (str == "TYPE_7") {
			addEnemy->AttackPattern = TYPE_7;
		}
		else if (str == "TYPE_8") {
			addEnemy->AttackPattern = TYPE_8;
		}
		else if (str == "TYPE_9") {
			addEnemy->AttackPattern = TYPE_9;
		}
		pAttrib = pAttrib->Next();

		str = pAttrib->Value(); // 이동 패턴----------------------
		if (str == "MOVE_R_L") {
			addEnemy->MovePattern = MOVE_R_L;
		}
		else if (str == "MOVE_R_L_FS") {
			addEnemy->MovePattern = MOVE_R_L_FS;
		}
		else if (str == "MOVE_R_L_ST") {
			addEnemy->MovePattern = MOVE_R_L_ST;
		}
		else if (str == "MOVE_U_D") {
			addEnemy->MovePattern = MOVE_U_D;
		}
		else if (str == "MOVE_U_D_FS") {
			addEnemy->MovePattern = MOVE_U_D_FS;
		}
		else if (str == "MOVE_U_D_ST") {
			addEnemy->MovePattern = MOVE_U_D_ST;
		}
		else if (str == "MOVE_D_U") {
			addEnemy->MovePattern = MOVE_D_U;
		}
		else if (str == "MOVE_D_U_FS") {
			addEnemy->MovePattern = MOVE_D_U_FS;
		}
		else if (str == "MOVE_D_U_ST") {
			addEnemy->MovePattern = MOVE_D_U_ST;
		}
		else if (str == "MOVE_RU_LD") {
			addEnemy->MovePattern = MOVE_RU_LD;
		}
		else if (str == "MOVE_C_R_L") {
			addEnemy->MovePattern = MOVE_C_R_L;
		}
		pAttrib = pAttrib->Next();

		addEnemy->nYPos = atoi(pAttrib->Value()); // YPos-----
		pAttrib = pAttrib->Next();

		addEnemy->nXPos = atoi(pAttrib->Value()); // XPos---------
		pAttrib = pAttrib->Next();

		addEnemy->nHP = atoi(pAttrib->Value()); // HP----------
		pAttrib = pAttrib->Next();

		str = pAttrib->Value(); // 적 유닛 종류-------------------
		if (str == "NOMAL_NECO") {
			addEnemy->EnemyType = NOMAL_NECO;
		}
		else if (str == "GRAY_NECO") {
			addEnemy->EnemyType = GRAY_NECO;
		}
		else if (str == "MECA_GIRL") {
			addEnemy->EnemyType = MECA_GIRL;
		}

		StageInfo.push_back(addEnemy);

		pElem = pElem->NextSiblingElement();
	}
}