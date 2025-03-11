#include "MoveComponent.h"


MoveComponent::MoveComponent(ActorClass* owner, float gravity, int	updateOrder)
	:Component(owner, updateOrder)
	, _OldPosition(owner->GetPosition())
	, _Velocity(VGet(0, 0, 0))
	, _Gravity(gravity)
	, _FallLimit(0)
{
}

MoveComponent::~MoveComponent()
{
}

void MoveComponent::Update()
{
	float gravity = 0.04 * _Owner->GetMode()->GetStepTm() * _Gravity;

	if (_FallLimit != 0)
	{
		if (GetVelocity().y < -_FallLimit)
		{
			gravity = 0;
		}
	}
	SetVelocity(VAdd(GetVelocity(), VGet(0, -gravity, 0)));

	if (_Stand == FALSE) {
	}
	_Stand = FALSE;
	_OldPosition = _Owner->GetPosition();
	float deltaTime = _Owner->GetMode()->GetStepTm();
	_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(_Velocity,deltaTime / 20)));
}
