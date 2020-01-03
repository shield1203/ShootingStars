#include"CEffect.h"

CEffect::CEffect(EFFECT_STATE State) {
	On = false;
	state = State;

	dwAniTime = 0;
	dwAni = 0;

	nXPos = 0;
	nYPos = 0;
}

CEffect::~CEffect() {
	
}