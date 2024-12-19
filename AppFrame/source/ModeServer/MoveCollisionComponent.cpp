#include "MoveCollisionComponent.h"
#include "ActorClass.h"
#include "ModelComponent.h"
#include "MoveComponent.h"

MoveCollisionComponent::MoveCollisionComponent(class ActorClass* owner, VECTOR pos, VECTOR size, int type, bool move, bool active, int handle)
	:Component(owner)
	, Pos(pos), Size(size), Type(type), isMove(move), isActive(active), Handle(handle)
{
	_Owner->GetMode()->AddMCollision(this);

	if (isMove == TRUE) {
		MoveComponent* moveComp = _Owner->GetComponent<MoveComponent>();
		_Owner->SetMove(VGet(0, 0, 0));
	}


	if (Handle != -1) { return; }
	
	ModelComponent* modelComp = _Owner->GetComponent<ModelComponent>();
	if (modelComp != nullptr) {
		switch (Type) {
		case 0:

			Handle = modelComp->GetHandle();
			break;

		default:
			Handle = -1;
			break;

		}
	}
	else {
		Handle = -1; // 適切なデフォルト値を設定
	}
	//*/
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