#pragma once
#include "appframe.h"
#include "ModeGame.h"
#include "EnemyStruct.h"


class EnemyActor : public ActorClass {
public:

	EnemyActor(class ModeBase* mode,VECTOR pos = VGet(0,0,0));
	virtual ~EnemyActor();


	void Init();
	void UpdateActor() override;

	
	class ModeGame* GetMode() override { return static_cast<ModeGame*>(_Mode); }
	class MoveComponent* GetInput() { return _Input; }

	
private:


	class MoveComponent* _Input;
	
	class ModelComponent* _Model;
	class MoveCollisionComponent* _MCollision;
	class HitCollisionComponent* _HCollision;



};

