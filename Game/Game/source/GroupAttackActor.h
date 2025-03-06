#pragma once
#include "appframe.h"
#include "PlayerActor.h"

class GroupSpawnerActor : public ActorClass {
public:
	struct SPAWNER_DATA {
		int max_popcount;			// 最大同時出現数
		int max_pop;				// 最大生成数
		int pop_range;				// 生成範囲
		int pop_time;				// 生成リキャスト時間
	};
	GroupSpawnerActor(class ModeBase* mode, VECTOR pos = VGet(0, 0, 0));
	virtual ~GroupSpawnerActor();
	void Init();
	void UpdateActor() override;
	void DecrementPopCnt() { _PopCnt--; }
	void SetData(SPAWNER_DATA data) { _Data = data; }
	void AddPopPos(VECTOR pos) { _PopPos.push_back(pos); }

	void SetHCollision(class HitCollisionComponent* hcol) { delete _HCollision; _HCollision = hcol; }

private:
	class PlayerActor* _Player[2];

	
	class HitCollisionComponent* _HCollision;
	int _TmCnt;						// 時間
	int _PopCnt;					// 生成数
	int _TotalPopCnt;				// 総生成数
	std::vector<VECTOR> _PopPos;	// 生成位置
	SPAWNER_DATA _Data;

	bool _Active;
};

class MHitCollisionComponent : public HitCollisionComponent {
public:
	MHitCollisionComponent(class ActorClass* owner, ModelComponent* model, VECTOR pos = VGet(0, 0, 0), VECTOR size = VGet(1, 1, 1)
		, int type = 0, bool move = FALSE, bool active = TRUE, int handle = -1);

	std::deque<HitCollisionComponent*>& IsHit() override;



};
	
