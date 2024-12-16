#pragma once

#include "../ActorClass.h"
#include <string>

class UIChipClass :public ActorClass {
public:
	struct UIData {
		VECTOR scale;
		float angle;
		bool turn;
	};

	UIChipClass(class ModeBase* mode, VECTOR pos, const TCHAR* fname,int draworder = 100);

	void ResetScale() { _Data.scale = VGet(1,1,1); }
	UIData* GetUIData() { return &_Data; }
	void ChangeImage(int num) { _Num = num; }
	int GetImageNum()const { return _Num; }
	void AddImage(std::string fname);
	int GetType() { return _DrawType; }
	void SetType(int type) { _DrawType = type; }
protected:
	UIData _Data;
	int _DrawType;
	int _Num;

	class UIChipSpriteComponent* _Sp;
};

