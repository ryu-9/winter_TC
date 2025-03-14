#pragma once
#include "appframe.h"

class SnowComponent : public SpriteComponent
{
public:
	SnowComponent(class ActorClass* owner, MV1_COLL_RESULT_POLY m, bool flag0, bool flag1, bool flag2);
	virtual ~SnowComponent();
	void Draw() override;

	void AddMoveCollision(MoveCollisionComponent* mc) { _MCList.push_back(mc); }
	void AddMoveCollision2(MoveCollisionComponent* mc) { _MCList2.push_back(mc); }

private:
	VERTEX3D* _Snow;
	VERTEX3D _LowSnow[6];
	int _SnowSize;
	unsigned short* _Index;
	unsigned short _LowIndex[21];
	int _IndexSize;
	float* _Height;
	VECTOR _Normal;
	int _Longest;


	std::deque<MoveCollisionComponent*> _MCList;
	std::deque<MoveCollisionComponent*> _MCList2;
	std::deque<MoveCollisionComponent*> _OldMCList;

	int _Split;
	int _Handle;

	bool _Flag;

	float _Area;

};

