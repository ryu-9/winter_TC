#pragma once
#include "Component.h"
#include "SpriteComponent.h"

class EffectComponent : public Component {
public:
	EffectComponent(class ActorClass* owner, const char* file);
	virtual ~EffectComponent();

	void Update() override;
	
	int GetEffectHandle() { return _EffectHandle; }

private:

	int _EffectHandle;
};

