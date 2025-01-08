#include "MoveCollisionComponent.h"
#include "ActorClass.h"
#include "ModelComponent.h"
#include "MoveComponent.h"
#include "../AppFrame/source/ModelServer/ModelServer.h"

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

			
		case 6:
			Handle = ModelServer::GetInstance()->Add("res/Collision/box.mv1");
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
				flag = FALSE;
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
				//case 0: // メッシュとメッシュ

					//break;

				case 1: // メッシュと線分	
					
					if (MV1CollCheck_Line(coll[0]->GetHandle(), 0, coll[1]->GetPosition(), VAdd(coll[1]->GetPosition(), coll[1]->GetSize())).HitFlag == TRUE) {
						flag = TRUE;
					}
					break;

				case 2: // 線分と線分
					break;

				case 4: // メッシュと球
					if (MV1CollCheck_Sphere(coll[0]->GetHandle(), 0, coll[1]->GetPosition(), VSize( coll[1]->GetSize())).HitNum > 0) {
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

				default:
					if (MV1CollCheck_Sphere(coll[0]->GetHandle(), 0, coll[1]->GetPosition(), VSize(coll[1]->GetSize())).HitNum > 0) {
						flag = TRUE;
					}
					break;
				
				}
				if (flag == TRUE) {

					if (coll[1]->isMove == TRUE) {
					}
					else {
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

void MoveCollisionComponent::DebugDraw()
{
	if (isActive == FALSE) {
		return;
	}
	if (flag == TRUE) {
		DrawFormatString(0, 0, GetColor(255, 255, 255), "Hit");
	}
	switch (Type) {

		// collisionの形　0: メッシュ 1:線分 2:球 3:カプセル 4:円柱  5:四角錐 6:直方体

	case 1:
		DrawLine3D(VAdd(GetPosition(), VGet(0, 0, 0)), VAdd(GetPosition(), VGet(0, 0, GetSize().z)), GetColor(255, 255, 255));
		break;

	case 2:
		DrawSphere3D(GetPosition(), VSize(GetSize()), 16, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
		break;


	default:
		MV1DrawModel(Handle);
		break;

	}
}
