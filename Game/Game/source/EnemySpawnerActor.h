#pragma once
#include "appframe.h"


class EnemyActor;

class EnemySpawnerActor : public ActorClass {
public:
	struct SPAWNER_DATA {
		int max_popcount;			// 最大同時出現数
		int max_pop;				// 最大生成数
		int pop_range;				// 生成範囲
		int pop_time;				// 生成リキャスト時間
		float active_range;			// アクティブ範囲
		float reset_range;			// リセット範囲
		float reset_time;			// リセット時間
	};

	EnemySpawnerActor(class ModeBase* mode, VECTOR pos = VGet(0, 0, 0),bool groupflag = false,bool resetflag = false);
	virtual ~EnemySpawnerActor();

	void Init();
	void UpdateActor() override;
	void DecrementPopCnt() { _PopCnt--; }
	void SetData(SPAWNER_DATA data) { _Data = data; }
	void SetType(int type) { _Type = type; }
	void SetCol(int col) { _Col = col; }
private:
	class PlayerActor* _Player[2];
	int _TmCnt;						// 時間
	int _PopCnt;					// 生成数
	int _TotalPopCnt;				// 総生成数
	bool _GroupFlag;				// 集団戦フラグ
	bool _ResetFlag;				// リセット有無フラグ
	int _Type;						// 敵の種類
	int _Col;						// 敵の色
	SPAWNER_DATA _Data;

	std::vector<VECTOR> _SpawnerPos;
};

