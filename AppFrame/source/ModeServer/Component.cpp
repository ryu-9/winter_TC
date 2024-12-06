#include "Component.h"
#include "ActorClass.h"

Component::Component(class ActorClass* owner, int updateOrder)
	:_Owner(owner)
	, _UpdateOrder(updateOrder)
{
	_Owner->AddComponent(this);
}

Component::~Component() {
	_Owner->RemoveComponent(this);
}

void Component::Update() {

}