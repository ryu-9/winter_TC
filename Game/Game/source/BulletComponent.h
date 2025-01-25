#pragma once
#include "appframe.h"

class BulletComponent : public Component{
public:
	BulletComponent(class ActorClass* owner);
	virtual ~BulletComponent();

	void ProcessInput() override;
	void Update() override;
	void SetSpeed(float speed) { _Speed = speed; }
	void SetGoalPos(const VECTOR& pos) { _GoalPos = pos; }

private:
	float _Speed;
	VECTOR _Dir;
	VECTOR _StartPos;
	VECTOR _GoalPos;

};

