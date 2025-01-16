#pragma once
#include "appframe.h"

class PlayerMoveComponent : public MoveComponent
{
public:
	PlayerMoveComponent(class PlayerActor* owner, int updateOrder = 10);
	virtual ~PlayerMoveComponent();

	void ProcessInput() override;

	void Receive(int message);
private:
	class PlayerActor* _pOwner;
	int _DashTime;
	VECTOR _DashDir;
	float _colSubY;	// コリジョン判定時のY補正(腰位置）

};

