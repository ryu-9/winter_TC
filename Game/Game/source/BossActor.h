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
	void SetModel(class ModelComponent* model,int index) { delete _Model[index]; _Model[index] = model; }
	void SetMoveCollision(class MoveCollisionComponent* mcol) { delete _MCollision; _MCollision = mcol; }
	void SetHitCollision(class HitCollisionComponent* hcol) { delete _HCollision; _HCollision = hcol; }

private:
	
	enum ACTION {
		WAIT,
		PUNCH,
		BULLET,
		BEAM,
		DAMAGE,
		DIE,
		COOLDOWN,
		NUM
	};

	typedef bool(BossActor::* ACTIONFUNCTION)(); // メンバ関数の場合
	ACTIONFUNCTION _ActionFunc[(int)ACTION::NUM] = {
		&BossActor::Wait,
		&BossActor::Punch,
		&BossActor::Bullet,
		&BossActor::Beam,
		&BossActor::Wait,
		&BossActor::Wait,
		&BossActor::CoolDown
	};

	bool ChangeAnim(ACTION a);
	bool ChangeAction(ACTION a);

	bool Wait();
	bool Punch();
	bool Bullet();
	bool Beam();
	bool CoolDown();

	class MoveComponent* _Input;
	class ModelComponent* _Model[2];
	
	class MoveCollisionComponent* _MCollision;
	class HitCollisionComponent* _HCollision;


	int _ActTime;				// 現在のアクション時間
	int _ActTotalTime;			// 最大アクション時間
	int _CurrentTime;			// 現在の時間

	ACTION _Action;				// 現アニメーション
	float _AnimTime;			// 再生時間
	float _AnimTotalTime;		// 合計時間
	int _AnimIndex;				// インデックス
	bool _AnimChangingflag;		// アニメーション変更中フラグ
	std::vector<int> _AnimMV1;	// モーションデータ


	struct ACTION_TIMELINE {
		ACTION action;
		int time;
	};
	std::vector<std::vector<ACTION_TIMELINE>> _ActionTimeline;

	int _ActionIndex;
	int _TimelineIndex;
};

