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
	if (isActive == FALSE || isMove == FALSE) {
		return;
	}
	for (auto mcoll : _Owner->GetMode()->GetMCollision()) {
		if (mcoll->GetIsActive() == TRUE) {
			if (mcoll != this) {
				bool flag = FALSE;
				int typeSum = 0;
				MoveCollisionComponent* coll[2];

				typeSum += Type * Type;
				if (typeSum < mcoll->GetType()* mcoll->GetType()) {
					coll[0] = this; coll[1] = mcoll;
				}
				else {
					coll[0] = mcoll; coll[1] = this;
				}
				switch (typeSum) {
				case 0: // メッシュとメッシュ

					break;

				case 1: // メッシュと線分	
					
					if (MV1CollCheck_Line(coll[0]->GetHandle(), 0, coll[1]->GetPosition(), VAdd(coll[1]->GetPosition(), coll[1]->GetSize())).HitFlag == TRUE) {
						flag = TRUE;
					}
					break;

				case 2: // 線分と線分
					break;

				case 4: // メッシュと球
					if (MV1CollCheck_Sphere(coll[0]->GetHandle(), 0, coll[1]->GetPosition(), VSize( coll[1]->GetSize())).Dim->HitFlag == TRUE) {
						flag = TRUE;
					}
					break;

				case 5: // 線分と球
					if (MV1CollCheck_Line(coll[0]->GetHandle(), 0, coll[1]->GetPosition(), VAdd(coll[1]->GetPosition(), coll[1]->GetSize())).HitFlag == TRUE) {
						flag = TRUE;
					}
					break;
				
				case 8: // 球と球
					if (VSize(VSub(coll[0]->GetPosition(), GetPosition())) < VSize(VAdd(GetSize(), coll[1]->GetSize()))) {
						flag = TRUE;
					}
					break; 
				
				}
				if (flag == TRUE) {
					if (coll[1]->isMove == TRUE) {
					}
										else {
						VECTOR move = VSub(GetPosition(), coll[1]->GetPosition());
						move = VNorm(move);
						move = VMulti(move, VSize(VSub(GetPosition(), coll[1]->GetPosition())));
						_Owner->SetMove(VAdd(_Owner->GetMove(), move));
					}
				
				}
			}
		}
	}
	
}
VECTOR MoveCollisionComponent::GetPosition() {
	return VAdd(_Owner->GetPosition(), VMulti(Pos, _Owner->GetSize())); 
}

VECTOR MoveCollisionComponent::GetSize() {
	return VMulti(Size, _Owner->GetSize()); 
}