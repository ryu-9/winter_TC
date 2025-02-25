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


	void SetMoveCollision(class MoveCollisionComponent* mcol) { delete _MCollision; _MCollision = mcol; }
	void SetHitCollision(class HitCollisionComponent* hcol) { delete _HCollision; _HCollision = hcol; }

	void ChangeMode(int mode);

	void Damage(float damage);
	bool IsMoved();
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

	class PlayerCursorComponent* _Cursor;

	int _PlayerNo;

	int _ModeNum; // 0:�ʏ� 1:�����g�@2:�㔼�g

	int _ChangeTime;

	int _Animation;
	float _AnimTime;
	float _AnimTotalTime;
	int _AnimIndex;
	int _AnimationModel[5];
	bool _AnimChangingflag;
};

