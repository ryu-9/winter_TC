//
// @file EnemySearchComponent.h
// @brief 距離と角度を基準にアクタを探すコンポーネント
// 


#pragma once
#include "EnemyStruct.h"

#include "appframe.h"
class EnemySearchComponent : public Component {
public:
	EnemySearchComponent(class ActorClass* owner,int updateOrder = 20);
	virtual ~EnemySearchComponent();

	void ProcessInput() override;



private:
	Enemy::SEARCH _SearchRef;
};

