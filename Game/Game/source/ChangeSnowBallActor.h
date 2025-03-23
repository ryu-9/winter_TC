#pragma once
#include "appframe.h"

class ChangeSnowBallActor : public ActorClass
{
public:
	ChangeSnowBallActor(class ModeBase* mode, ActorClass* owner, float size);
	virtual ~ChangeSnowBallActor();
	void UpdateActor() override;

private:
	 ModelComponent* _Model;
	ActorClass* _Owner;
	float _AnimTime;
	int _LifeTime;
	bool _Flag;
};

