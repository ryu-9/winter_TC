#include "MoveComponent.h"
#include "../ModeServer/Fpscontroller.h"


MoveComponent::MoveComponent(ActorClass* owner, int	updateOrder)
	:Component(owner, updateOrder)
	, _OldPosition(owner->GetPosition())
	, _Velocity(VGet(0, 0, 0))
{
}

MoveComponent::~MoveComponent()
{
}

void MoveComponent::Update()
{
	SetVelocity(VAdd(GetVelocity(), VGet(0, -0.2, 0)));
	if (_Stand == FALSE) {
	}
	_Stand = FALSE;
	_OldPosition = _Owner->GetPosition();
	float deltaTime = FpsController::GetInstance()->GetDeltaTime();
	_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(_Velocity,deltaTime / 20)));
}
