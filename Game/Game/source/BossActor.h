#pragma once
#include "appframe.h"
#include "ModeGame.h"
#include <queue>


class BossActor : public ActorClass{
public:
	BossActor(class ModeBase* mode, VECTOR pos = VGet(0, 0, 0));
	virtual ~BossActor();
	void UpdateActor() override;
	class ModeGame* GetMode() override { return static_cast<ModeGame*>(_Mode); }
	class MoveComponent* GetInput() { return _Input; }
	void SetInput(class MoveComponent* input) { delete _Input; _Input = input; }
	void SetModel(class ModelComponent* model,int index) { delete _Model[index]; _Model[index] = model; }
	void SetHitCollision(class HitCollisionComponent* hcol) { delete _HCollision; _HCollision = hcol; }


	enum ACTION {
		WAIT,			// 待機
		PUNCH,			// パンチ
		BULLET,			// 弾
		BEAM,			// ビーム
		PUNCH_FALL,		// パンチ(落下)
		DAMAGE,			// ダメージ
		DIE,			// 死亡
		ACTION_COUNT	// アクション数
	};
private:

	// アクション関数(関数ポインタ)
	typedef bool(BossActor::* ACTIONFUNCTION)();
	ACTIONFUNCTION _ActionFunc[(int)ACTION::ACTION_COUNT] = {
		&BossActor::Wait,
		&BossActor::Punch,
		&BossActor::Bullet,
		&BossActor::Beam,
		&BossActor::PunchFall,
		&BossActor::Damage,
		&BossActor::Die
	};

	// アクション関数(中身)
	bool Wait();
	bool Punch();
	bool Bullet();
	bool Beam();
	bool PunchFall();
	bool Damage();
	bool Die();


	void GeneratePunch();
	void GenerateBullet();
	void GenerateBeam();
	void GeneratePunchFall();
	void GenerateEnemy();

	bool ChangeAnim(ACTION a);
	bool ChangeAction(ACTION a);

	bool Rotate();

	class MoveComponent* _Input;
	class ModelComponent* _Model[2];
	
	class HitCollisionComponent* _HCollision;

	int _ActTime;						// 現在のアクション時間
	int _ActTotalTime;					// 最大アクション時間
	int _CurrentTime;					// 現在の時間
	int _GenerateCnt;					// 生成回数

	ACTION _Action;						// 現アニメーション
	float _AnimTime;					// 再生時間
	float _AnimTotalTime;				// 合計時間
	std::queue<int> _AnimIndex;			// インデックス
	float _AnimRate;					// アニメーション割合
	std::vector<int> _AnimMV1;			// モーションデータ
	std::vector<const char*> _AnimName;	// モーション名

	// タイムライン用構造体
	struct ACTION_TIMELINE {
		ACTION action;
		int time;
	};
	std::vector<std::vector<ACTION_TIMELINE>> _ActionTimeline;

	int _ActionIndex;				// タイムライン内アクション番号
	int _TimelineIndex;				// タイムライン番号

	int _HitPoint;					// ヒットポイント

	bool _UpdateRotf;				// 回転フラグ

};

