#pragma once
#include "appframe.h"

class BulletComponent : public Component{

	BulletComponent(class ActorClass* owner);
	virtual ~BulletComponent();

	void ProcessInput() override;
	void Update() override;
	void SetSpeed(float speed) { _Speed = speed; }

private:
	float _Speed;
	VECTOR _GoalPos;
};

