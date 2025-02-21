#pragma once
#include "appframe.h"

class PlayerMoveComponent : public MoveComponent
{
public:
	PlayerMoveComponent(class PlayerActor* owner, int updateOrder = 10);
	virtual ~PlayerMoveComponent();

	void ProcessInput() override;

	void Receive(int message);

	VECTOR GetDashDir() const { return _DashDir; }
	bool GetDashFlag() const { return _DashFlag; }
private:
	class PlayerActor* _pOwner;
	int _DashTime;
	bool _DashFlag;
	VECTOR _DashDir;
	float _colSubY;	// コリジョン判定時のY補正(腰位置）


};

