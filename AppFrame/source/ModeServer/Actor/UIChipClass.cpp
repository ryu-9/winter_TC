#include "UIChipClass.h"
#include "UIChipSpriteComponent.h"

UIChipClass::UIChipClass(ModeBase* mode, VECTOR pos,const TCHAR* fname, int draworder)
	:ActorClass(mode)
	, _Num(0)
{
	if (fname != "") {
		_Sp = new UIChipSpriteComponent(this, draworder);
		_Sp->SetImage(fname);
	}
	_Position = pos;
	_Data.scale = VGet(1, 1, 1);
	_Data.angle = 0;
	_Data.turn = false;
	_Data.blendMode = DX_BLENDMODE_NOBLEND;
	_Data.blendParam = 0;
}

void UIChipClass::AddImage(std::string fname) {
	_Sp->SetImage(fname.c_str());
}

