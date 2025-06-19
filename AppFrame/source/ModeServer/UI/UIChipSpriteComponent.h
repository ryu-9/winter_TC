#pragma once
#include "../SpriteComponent.h"
#include "UIChipClass.h"

class UIChipClass;
class UIChipSpriteComponent : public SpriteComponent
{
public:
	UIChipSpriteComponent(class UIChipClass* owner, int drawOrder = 100);
	void Draw() override;
	void SetImage(const TCHAR* filename);

	void Receive(int message) override;
protected:

	class UIChipClass* _UIc;


	struct CGData {
		VECTOR pos = VGet(0,0,0);
		int width;
		int height;
		int handle;
	};
	std::vector<CGData> _CGData;
};

