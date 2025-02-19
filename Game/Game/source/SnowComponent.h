#pragma once
#include "appframe.h"

class SnowComponent : public SpriteComponent
{
public:
	SnowComponent(class ActorClass* owner, MV1_COLL_RESULT_POLY m);
	virtual ~SnowComponent();
	void Draw() override;

	void AddMoveCollision(MoveCollisionComponent* mc) { _MCList.push_back(mc); }

private:
	VERTEX3D* _Snow;
	int _SnowSize;
	unsigned short* _Index;
	int _IndexSize;
	float* _Height;
	VECTOR _Normal;
	int _Longest;


	std::vector<MoveCollisionComponent*> _MCList;

	int _Split;

};

