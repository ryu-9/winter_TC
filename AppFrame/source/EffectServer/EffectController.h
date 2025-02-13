#pragma once

#include <deque>
#include "../ModeServer/ActorClass.h"
#include "EffectManager.h"
#include "../ModeServer/ModeBase.h"
#include "../AppFrame/source/ModeServer/ShadowMapSpriteComponent.h"


class EffectController : ActorClass
{
public:
	EffectController(ModeBase* mode);
	virtual	~EffectController();
	
	void Draw();

	static EffectController* _lpInstance;
	static EffectController* GetInstance() { return (EffectController*)_lpInstance; }

	void AddEffect(EffectManager* effect);
	void DelEffect(EffectManager* effect);

	EffectManager* GetEffectManager(SpriteComponent* sp);

	void AddShadowMap(int size = 1024, VECTOR dir = VGet(0, -1, 0), VECTOR target = VGet(0, 0, 0), int index = 0, float length = 100, int num = 0, int drawOrder = -1000000);


private:
	std::deque<EffectManager*> _EffectList;
	ModeBase* _Mode;
	std::deque<ShadowMapSpriteComponent*> _ShadowMapList;

};

