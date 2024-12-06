#pragma once
#include <tchar.h>
#include <vector>
#include "Component.h"
#include "Dxlib.h"
#include "ModeBase.h"

class SpriteComponent : public Component {
public:
	SpriteComponent(class ActorClass* owner, int drawOrder = 100);
	virtual ~SpriteComponent();

	virtual void Draw();
	virtual void SetImage() {};

	int GetDrawOrder() const { return _DrawOrder; }



protected:
	int _DrawOrder;
};