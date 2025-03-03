#pragma once
#include "appframe.h"
#include "ModeGame.h"
#include "EnemyStruct.h"

class EnemySpawnerActor;

class EnemyActor : public ActorClass {
public:

	EnemyActor(class ModeBase* mode,VECTOR pos = VGet(0,0,0), class EnemySpawnerActor* es = nullptr);
	virtual ~EnemyActor();


	void Init();
	void UpdateActor() override;

	
	class ModeGame* GetMode() override { return static_cast<ModeGame*>(_Mode); }
	class MoveComponent* GetInput() { return _Input; }

	void SetInput(class MoveComponent* input) { delete _Input; _Input = input; }
	void SetModel(class ModelComponent* model) { delete _Model; _Model = model; }
	void SetMoveCollision(class MoveCollisionComponent* mcol) { delete _MCollision; _MCollision = mcol; }
	void SetHitCollision(class HitCollisionComponent* hcol) { delete _HCollision; _HCollision = hcol; }
private:


	class MoveComponent* _Input;
	
	class ModelComponent* _Model;
	class MoveCollisionComponent* _MCollision;
	class HitCollisionComponent* _HCollision;

	class EnemySpawnerActor* _Spawner;

};

int Drawing(std::vector<int> w);