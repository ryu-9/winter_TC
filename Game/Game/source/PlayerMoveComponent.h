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

	DINPUT_JOYSTATE GetInput() const { return _Input; }
	int GetKey() const { return _Key; }
	int GetTrg() const { return _Trg; }

	void SetDashTime(int time) { _DashTime = time; }

	void SetDashDownTime(int time) { _DashDownTime = time; }	

	void SetIsActive(bool flag) { _IsActive = flag; }

private:
	class PlayerActor* _pOwner;
	int _DashTime;
	int _Key;
	int _Trg;
	bool _DashFlag;
	VECTOR _DashDir;
	float _colSubY;	// コリジョン判定時のY補正(腰位置）
	DINPUT_JOYSTATE _Input;


	bool _JumpFlag;
	
	int _DashDownTime;

	bool _IsActive;

};

