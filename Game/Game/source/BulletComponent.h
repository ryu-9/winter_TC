#pragma once
#include "appframe.h"

class BulletComponent : public MoveComponent{
public:
	BulletComponent(class ActorClass* owner,VECTOR pos,float speed);
	virtual ~BulletComponent();

	void ProcessInput() override;
	void Update() override;

	void SetSpeed(float speed) { _Speed = speed; }
	void SetGoalPos(const VECTOR& pos) { _GoalPos = pos; }
	void UpdateDir();
private:
	float _Speed;		// 速度
	float _Gravity;		// 重力
	VECTOR _Dir;		// 移動方向
	VECTOR _StartPos;	// 開始位置
	VECTOR _GoalPos;	// 目標位置
	int _Cnt;			// カウント
};

