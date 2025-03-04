#pragma once
#include "Component.h"
#include "Dxlib.h"
#include "ModeBase.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class ActorClass* owner, bool gravity = TRUE, int updateOrder = 100);
	virtual ~MoveComponent();

	void Update();

	VECTOR GetOldPosition() const { return _OldPosition; }
	void SetOldPosition(VECTOR pos) { _OldPosition = pos; }

	VECTOR GetVelocity() const { return _Velocity; }
	void SetVelocity(VECTOR vel) { _Velocity = vel; }

	bool GetStand() const { return _Stand; }
	void SetStand(bool stand) { _Stand = stand; }

	void SetGravity(bool gravity) { _Gravity = gravity; }
private:
	VECTOR _OldPosition;
	VECTOR _Velocity;

	bool _Stand;
	bool _Gravity;
};

