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
		VECTOR active_pos;			// エリア座標
		VECTOR active_size;			// エリアサイズ
	};
	GroupSpawnerActor(class ModeBase* mode, VECTOR pos = VGet(0, 0, 0));
	virtual ~GroupSpawnerActor();
	void Init();
	void UpdateActor() override;
	void DecrementPopCnt() { _PopCnt--; }
	void SetData(SPAWNER_DATA data) { _Data = data; }

private:
	class PlayerActor* _Player[2];
	int _TmCnt;						// 時間
	int _PopCnt;					// 生成数
	int _TotalPopCnt;				// 総生成数
	SPAWNER_DATA _Data;
};

