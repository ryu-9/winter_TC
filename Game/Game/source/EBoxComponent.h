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
	bool Rotate_Move();
	WAIT_ACTION _WaitAction;

	
	VECTOR _MoveDir;	// ˆÚ“®•ûŒü
	VECTOR _MFront;		// ‘O•ûŒü
	VECTOR _MUp;		// ã•ûŒü


	float _RollAngle;	// ‰ñ“]Šp“x
	int _RMTime;		// ‰ñ“]•b”

	
	float _HalfSize;
	float _Slanting;
};

