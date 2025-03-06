#pragma once
#include "Component.h"
#include "Dxlib.h"
#include "ModeBase.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class ActorClass* owner, float gravity = 1.0, int updateOrder = 100);
	virtual ~MoveComponent();

	void Update();

	VECTOR GetOldPosition() const { return _OldPosition; }
	void SetOldPosition(VECTOR pos) { _OldPosition = pos; }

	VECTOR GetVelocity() const { return _Velocity; }
	void SetVelocity(VECTOR vel) { _Velocity = vel; }

	bool GetStand() const { return _Stand; }
	void SetStand(bool stand) { _Stand = stand; }

	void SetGravity(float gravity) { _Gravity = gravity; }

	void SetFallLimit(float limit) { _FallLimit = limit; }

private:
	VECTOR _OldPosition;
	VECTOR _Velocity;

	bool _Stand;
	float _Gravity;

	float _FallLimit;
};

