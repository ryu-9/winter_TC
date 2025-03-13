#pragma once
#include "appframe.h"

class GimmickWallActor : public ActorClass
{
public:
	GimmickWallActor(ModeBase* mode, VECTOR pos, VECTOR size, VECTOR rot, int type, ActorClass* actor);
	~GimmickWallActor();
	void UpdateActor() override;

	void SetIsActive(bool flag);
	void SetActor(ActorClass* actor) { _Actor = actor; }

private:
	MoveCollisionComponent* _MCollision;
	ModelComponent* _Model;
	ActorClass* _Actor;
	bool _IsActive;
	float _Move;

};

