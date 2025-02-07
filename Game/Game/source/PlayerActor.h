#pragma once
#include "appframe.h"
#include "ModeGame.h" 

class PlayerActor : public ActorClass {
public:
	PlayerActor(class ModeBase* mode , int playerNo = 1);
	virtual ~PlayerActor();

	void UpdateActor() override;

	class ModeGame* GetMode() override { return static_cast<ModeGame*>(_Mode);}

	int GetModeNum() const { return _ModeNum; }

	int GetPlayerNo() const { return _PlayerNo; }
	
	void SetInput(class PlayerMoveComponent* input) { delete _Input; _Input = input; }
	class PlayerMoveComponent* GetInput() { return _Input; }

	void SetFriend(PlayerActor* fri) { _Friend = fri; }
	PlayerActor* GetFriend() { return _Friend; }

	void ChangeMode(int mode);

	void ChangeAnim(int a);

private:

	enum class anim {
		Change,
		Wait,
		Walk,
		Dash,
		Punch,
	};

	class PlayerMoveComponent* _Input;

	class PlayerActor* _Friend;

	class ModelComponent* _BallModel;
	class ModelComponent* _TopModel;
	class ModelComponent* _BottomModel;

	class MoveCollisionComponent* _MCollision;
	class MoveCollisionComponent* _MCollision2;

	class HitCollisionComponent* _HCollision;

	int _PlayerNo;

	int _ModeNum; // 0:í èÌ 1:â∫îºêgÅ@2:è„îºêg

	int _ChangeTime;

	int _Animation;
	float _AnimTime;
	float _AnimTotalTime;
	int _AnimIndex;
	int _AnimationModel;
};

