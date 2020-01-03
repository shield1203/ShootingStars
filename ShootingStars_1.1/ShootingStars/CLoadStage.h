#pragma once
#include<Windows.h>
#include"tinyxml.h"
#include<vector>
#include"CStage.h"
#include<string>

using namespace std;

class CLoadStage {
public:
	void LoadFile(vector<CStage*> &StageInfo, const char* strFileName);
};