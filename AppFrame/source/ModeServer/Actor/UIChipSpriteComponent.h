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
protected:

	class UIChipClass* _UIc;


	struct CGData {
		int width;
		int height;
		int handle;
	};
	std::vector<CGData> _CGData;
};

