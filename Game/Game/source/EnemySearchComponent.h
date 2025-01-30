#pragma once

//
// @explanation:
// エネミーが探索を行うコンポーネント
//

#include "appframe.h"
class EnemySearchComponent : public Component {
public:
	EnemySearchComponent(class ActorClass* owner,int updateOrder = 20);
	virtual ~EnemySearchComponent();

	void ProcessInput() override;

	struct SEARCH
	{
		float dist;		// 探索範囲
		float angle;	// 探索角度
	};

private:
	SEARCH _SearchRef;
};

