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

	bool Search(std::vector<ActorClass*> target);

	EnemyActor* _En;

	std::vector<ActorClass*> _Target;
	Enemy::SEARCH _SearchRef;
};

