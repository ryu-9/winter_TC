#pragma once

#include <deque>
#include "../ModeServer/ActorClass.h"
#include "EffectManager.h"
#include "../ModeServer/ModeBase.h"


class EffectController : ActorClass
{
public:
	EffectController(ModeBase* mode);
	virtual	~EffectController();
	
	void Draw();

	void AddEffect(EffectManager* effect);
	void DelEffect(EffectManager* effect);

	EffectManager* GetEffectManager(SpriteComponent* sp);


private:
	std::deque<EffectManager*> _EffectList;
	ModeBase* _Mode;

};

