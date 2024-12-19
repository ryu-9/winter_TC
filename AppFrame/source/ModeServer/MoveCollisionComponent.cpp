#include "MoveCollisionComponent.h"
#include "ActorClass.h"
#include "ModelComponent.h"

MoveCollisionComponent::MoveCollisionComponent(class ActorClass* owner, VECTOR pos, VECTOR size, int type, bool move, bool active, int handle)
	:Component(owner)
	, Pos(pos), Size(size), Type(type), isMove(move), isActive(active), Handle(handle)
{
	_Owner->GetMode()->AddMCollision(this);
	if (Handle != -1) { return; }
	ModelComponent *tmp = nullptr;
	switch (Type) {
	case 0:

		Handle = dynamic_cast<ModelComponent*>(_Owner->GetComponent(tmp))->GetHandle();
		break;

	default:
		Handle = -1;
		break;
	
	}
	delete tmp;
}


MoveCollisionComponent::~MoveCollisionComponent()
{
	_Owner->GetMode()->RemoveMCollision(this);
}

void MoveCollisionComponent::Update()
{
}
VECTOR MoveCollisionComponent::GetPosition() {
	return VAdd(_Owner->GetPosition(), VMulti(Pos, _Owner->GetSize())); 
}

VECTOR MoveCollisionComponent::GetSize() {
	return VMulti(Size, _Owner->GetSize()); 
}