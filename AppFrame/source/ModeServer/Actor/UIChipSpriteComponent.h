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
//	int GetImageSize() { return _cgHandle.size(); }
	void SetBlendMode(int type) { _BlendMode = type; }
	void SetBlendPalam(int palam) { _BlendPalam = palam; }
protected:
	int _BlendMode;
	int _BlendPalam;
	class UIChipClass* _UIc;


	struct CGData {
		int width;
		int height;
		int handle;
	};
	std::vector<CGData> _CGData;
};

