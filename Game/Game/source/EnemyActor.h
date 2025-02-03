#pragma once
#include "appframe.h"
#include "ModeGame.h"
#include "EnemyStruct.h"


class EnemyActor : public ActorClass {
public:

	EnemyActor(class ModeBase* mode);
	virtual ~EnemyActor();


	void Init();
	void UpdateActor() override;

	
	class ModeGame* GetMode() override { return static_cast<ModeGame*>(_Mode); }

	bool Search(std::vector<ActorClass*> target);
private:



	
	class ModelComponent* _Model;
	class MoveCollisionComponent* _MCollision;




};

