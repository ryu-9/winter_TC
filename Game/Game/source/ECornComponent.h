#pragma once
#include "EnemyComponent.h"
class ECornComponent : public EnemyComponent {
public:
	ECornComponent(ActorClass* owner);
	~ECornComponent();
	void ProcessInput() override;

protected:
	virtual bool Attack() override;
	void Jump();

};

