#pragma once
#include "appframe.h"
class EnemyAttackComponent : public Component{
public:
	EnemyAttackComponent(class ActorClass* owner);
	virtual ~EnemyAttackComponent();

	void ProcessInput() override;
	void Update() override;

private:
	float _Interval;

};

