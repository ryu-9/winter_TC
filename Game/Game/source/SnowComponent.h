#pragma once
#include "appframe.h"

class SnowComponent : public SpriteComponent
{
public:
	SnowComponent(class ActorClass* owner, MV1_COLL_RESULT_POLY m, bool flag0, bool flag1, bool flag2);
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


	std::deque<MoveCollisionComponent*> _MCList;
	std::deque<MoveCollisionComponent*> _OldMCList;

	int _Split;
	int _Handle;

};

