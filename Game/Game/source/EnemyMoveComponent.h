#pragma once
#include "appframe.h"

class EnemyMoveComponent : public MoveComponent {
public:
	EnemyMoveComponent(class ActorClass* owner);
	virtual ~EnemyMoveComponent();

	void ProcessInput() override;



};

