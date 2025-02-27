#include "MoveComponent.h"
#include "../ModeServer/Fpscontroller.h"


MoveComponent::MoveComponent(ActorClass* owner, bool gravity, int	updateOrder)
	:Component(owner, updateOrder)
	, _OldPosition(owner->GetPosition())
	, _Velocity(VGet(0, 0, 0))
	, _Gravity(gravity)
{
}

MoveComponent::~MoveComponent()
{
}

void MoveComponent::Update()
{
	if (_Gravity) {
		float gravity = 0.04 * FpsController::GetInstance()->GetDeltaTime();
		SetVelocity(VAdd(GetVelocity(), VGet(0, -gravity, 0)));
	}

	if (_Stand == FALSE) {
	}
	_Stand = FALSE;
	_OldPosition = _Owner->GetPosition();
	float deltaTime = FpsController::GetInstance()->GetDeltaTime();
	_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(_Velocity,deltaTime / 20)));
}
