#pragma once
#include "appframe.h"
#include "ModeGame.h"


class BossActor : public ActorClass{
public:
	BossActor(class ModeBase* mode, VECTOR pos = VGet(0, 0, 0));
	virtual ~BossActor();
	void Init();
	void UpdateActor() override;
	class ModeGame* GetMode() override { return static_cast<ModeGame*>(_Mode); }
	class MoveComponent* GetInput() { return _Input; }
	void SetInput(class MoveComponent* input) { delete _Input; _Input = input; }
	void SetModel(class ModelComponent* model) { delete _Model; _Model = model; }
	void SetMoveCollision(class MoveCollisionComponent* mcol) { delete _MCollision; _MCollision = mcol; }
	void SetHitCollision(class HitCollisionComponent* hcol) { delete _HCollision; _HCollision = hcol; }

private:
	enum ACTION {
		NONE,
	};

	enum ANIM {
		CHANGE,
		WAIT,
		WALK,
		DASH,
		PUNCH,
		NUM
	};

	bool ChangeAnim(ANIM a);

	class MoveComponent* _Input;
	class ModelComponent* _Model;
	class MoveCollisionComponent* _MCollision;
	class HitCollisionComponent* _HCollision;

	

	ANIM _Animation;
	float _AnimTime;
	float _AnimTotalTime;
	int _AnimIndex;
	bool _AnimChangingflag;
	std::vector<int> _AnimMV1;

};

