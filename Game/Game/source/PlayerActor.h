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


	void SetMoveCollision(class PlayerMoveCollisionComponent* mcol) { delete _MCollision; _MCollision = mcol; }
	void SetHitCollision(class HitCollisionComponent* hcol) { delete _HCollision; _HCollision = hcol; }

	void ChangeMode(int mode);

	void Damage(float damage, int time = 1000);
	void KnockBack(VECTOR dir, float power);
	bool IsMoved();
	void ChangeAnim(int a);

	void SetChangeFlag(bool flag) { _ChangeFlag = flag; }

	void SetChangeTime(int time) { _ChangeTime = time; }

	int GetKnockBackTime() { return _KnockBackTime; }

	void Init();

	void SetItemNum(int num) { _ItemNum = num; }
	void DropItem(VECTOR dir, int num);

	void SetDropItem(int i, int n) { _Item[i] = n; }

	void AddSize(float size, bool flag = false);

	int GetSeparateTime() { return _SeparateTime; }

	void SetStartPos(VECTOR pos) { _StartPos = pos; }

	int GetInvincibleTime() { return _InvincibleTime; }

private:

	enum class anim {
		Change,
		Wait,
		Walk,
		Dash,
		Punch,
		Laser,
		Blade,
		Daikanpa,
		Damage,
		Num
	};

	std::map<int, float>_AnimationRate;


	class PlayerMoveComponent* _Input;

	class PlayerActor* _Friend;

	class ModelComponent* _BallModel;
	class ModelComponent* _TopModel;
	int _TopModelHandle[4];

	class ModelComponent* _BottomModel;
	int _BottomModelHandle[4];

	class PlayerMoveCollisionComponent* _MCollision;
	class MoveCollisionComponent* _MCollision2;

	class HitCollisionComponent* _HCollision;

	class PlayerCursorComponent* _Cursor;

	int _PlayerNo;

	int _ModeNum; // 0:雪玉 1:上半身 2:下半身 3:死亡

	int _ChangeTime;

	int _Animation;
	float _AnimTime;
	float _AnimTotalTime;
	int _AnimIndex;
	int _AnimationModel[(int)anim::Num + 2];
	bool _AnimChangingflag;

	int _PunchIndex[2];

	bool _PunchFlag;
	int _InvincibleTime;

	bool _LavaFlag;

	bool _ChangeFlag;

	int _KnockBackTime;

	int _ItemNum;

	int _FallTime;

	int _SeparateTime;

	VECTOR _StartPos;

	int _Item[2] = { 0,0 };

	int _DeadTime;

};

