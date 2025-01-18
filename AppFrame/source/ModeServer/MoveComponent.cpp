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
	SetVelocity(VAdd(GetVelocity(), VGet(0, -1, 0)));
	if (_Stand == FALSE) {
		
	}
	_Stand = FALSE;
	_OldPosition = _Owner->GetPosition();
	MoveCollisionComponent* mcc = _Owner->GetComponent<MoveCollisionComponent>();
	_Owner->SetPosition(VAdd(_Owner->GetPosition(), _Velocity));
	//mcc->Process();
	//return;
	if(mcc!=nullptr) {
		float acc = mcc->GetAccuracy();
		int num = VSize(_Velocity) / acc;
		VECTOR move = VScale(VNorm(_Velocity), acc);
		for (int i = 0; i < num; i++) {
			mcc->Process();
			_Owner->SetPosition(VAdd(_Owner->GetPosition(), move));
		}
		mcc->Process();
		_Owner->SetPosition(VAdd(_Owner->GetPosition(), VSub(_Velocity,VScale(move,num))));
		mcc->Process();

	}
	else {
		_Owner->SetPosition(VAdd(_Owner->GetPosition(), _Velocity));
	}
}
