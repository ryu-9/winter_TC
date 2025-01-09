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
			Handle = ModelServer::GetInstance()->Add("res/cube.mv1");
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
				VECTOR move = VGet(0,0,0);
				MoveCollisionComponent* coll[2];

				typeSum += Type * Type;
				if (typeSum < mcoll->GetType()* mcoll->GetType()) {
					coll[0] = this; coll[1] = mcoll;
				}
				else {
					coll[0] = mcoll; coll[1] = this;
				}
				typeSum += mcoll->GetType() * mcoll->GetType();
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

				case 40: // 球と直方体
					if (MV1CollCheck_Sphere(coll[0]->GetHandle(), 0, coll[1]->GetPosition(), VSize(coll[1]->GetSize())).HitNum > 0) {
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

					switch (typeSum) {

					case 1: // メッシュと線分
						break;

					case 8: // 球と球
						break;


					case 40: // 球と直方体
					{
						VECTOR dir = VSub(_Owner->GetComponent<MoveComponent>()->GetOldPosition(), mcoll->GetPosition());
						float x = VDot(coll[0]->GetRight(), dir), y = VDot(coll[0]->GetUp(), dir), z = VDot(coll[0]->GetFront(), dir);
						if (x < 0) { x *= -1; } if (y < 0) { y *= -1; } if (z < 0) { z *= -1; }
						int n = 0; if (x > 0) { n = 1; }
						if (y > x) {
							n = 2;
							if (z > y) { n = 3; }
						}
						else if (z > x) { n = 3; }
						float dist = 0;
						switch (n) {
						case 1: // x
							dist = VDot(VSub(GetPosition(), mcoll->GetPosition()), GetRight());
							move = VScale(GetRight(), dist);
							break;

						case 2: // y
							dist = VDot(VSub(GetPosition(), mcoll->GetPosition()), GetUp());
							move = VScale(GetUp(), dist);
							break;

						case 3: // z
							dist = VDot(VSub(GetPosition(), mcoll->GetPosition()), GetFront());
							move = VScale(GetFront(), dist);
							break;

						default:
							break;
						}
						break;
					}
					default:
						break;
					}



					if (mcoll->isMove == TRUE) {
					}
					else {
						_Owner->SetPosition(VAdd(_Owner->GetPosition(), move));
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

VECTOR MoveCollisionComponent::GetUp()
{
	return _Owner->GetComponent<ModelComponent>()->GetUp();
}

VECTOR MoveCollisionComponent::GetFront()
{
	return _Owner->GetComponent<ModelComponent>()->GetFront();
}

VECTOR MoveCollisionComponent::GetRight()
{
	return VCross(GetUp(),GetFront());
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
