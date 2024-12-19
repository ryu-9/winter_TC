#include "MoveCollisionComponent.h"
#include "ActorClass.h"

MoveCollisionComponent::MoveCollisionComponent(class ActorClass* owner, VECTOR pos, VECTOR size, int type, bool move, bool isMove)
	:Component(owner)
	, Pos(pos), Size(size), Type(type), isMove(move), isActive(TRUE)
{
	_Owner->GetMode()->AddMCollision(this);
	switch (Type) {
	case 0:
		//Handle = _Owner->GetMode()->GetHandle();
	
	}
}

MoveCollisionComponent::MoveCollisionComponent(ActorClass* owner, VECTOR pos, VECTOR size, int type, int handle, bool move, bool isMove)
	:Component(owner)
	, Pos(pos), Size(size), Type(type), Handle(handle), isMove(move), isActive(TRUE)
{
	_Owner->GetMode()->AddMCollision(this);
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