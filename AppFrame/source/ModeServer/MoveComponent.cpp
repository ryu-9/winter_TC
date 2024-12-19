#include "MoveComponent.h"

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
	_OldPosition = _Owner->GetPosition();
	_Owner->SetPosition(VAdd(_Owner->GetPosition(), _Velocity));
}
