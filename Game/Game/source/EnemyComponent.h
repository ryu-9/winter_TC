#pragma once
#include "appframe.h"
#include "EnemyActor.h"

class EnemyComponent : public Component{
public:
	EnemyComponent(ActorClass* owner);
	~EnemyComponent();
	void ProcessInput();
	void Update();

private:
	EnemyActor* _EnOwner;

};

