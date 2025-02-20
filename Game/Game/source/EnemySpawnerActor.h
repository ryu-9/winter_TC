#pragma once
#include "appframe.h"
class EnemySpawnerActor : public ActorClass {
public:

	EnemySpawnerActor(class ModeBase* mode, VECTOR pos = VGet(0, 0, 0));
	virtual ~EnemySpawnerActor();

	void Init();
	void UpdateActor() override;

private:

};

