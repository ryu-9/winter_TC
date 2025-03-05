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
		WAIT,
		PUNCH,
		BULLET,
		BEAM,
		DAMAGE,
		COOLDOWN,
		NUM
	};

	bool Punch();

	bool ChangeAnim(ACTION a);
	bool ChangeMotion(ACTION a);
	bool CoolDown();

	class MoveComponent* _Input;
	class ModelComponent* _Model;
	class MoveCollisionComponent* _MCollision;
	class HitCollisionComponent* _HCollision;


	int _ActTime;				// 現在のアクション時間
	int _ActTotalTime;			// 最大アクション時間
	

	ACTION _Action;			// 現アニメーション
	float _AnimTime;			// 再生時間
	float _AnimTotalTime;		// 合計時間
	int _AnimIndex;				// インデックス
	bool _AnimChangingflag;		// アニメーション変更中フラグ
	std::vector<int> _AnimMV1;	// モーションデータ

};

