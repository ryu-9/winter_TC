#pragma once
#include "EnemyComponent.h"
class EBoxComponent : public EnemyComponent {
public:
	EBoxComponent(ActorClass* owner);
	~EBoxComponent();
	void ProcessInput() override;
	enum WAIT_ACTION {
		NON,
		MOVE,
	};

protected:
	bool Attack() override;
	bool Move() override;
	WAIT_ACTION _WaitAction;

	
	VECTOR _MoveDir;	// 移動方向
	VECTOR _MFront;		// 前方向
	VECTOR _MLeft;		// 横方向

	
	VECTOR _StartRot;	// 回転開始角度
	VECTOR _AddRot;		// 回転終了角度
	float _RollAngle;	// 回転角度

	
	float _HalfSize;	// 辺の長さの半分
	float _Slanting;	// 斜め
};

