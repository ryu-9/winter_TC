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
	MV1SetPosition(Handle, GetPosition());
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
					if (MV1CollCheck_Sphere(coll[1]->GetHandle(), 0, coll[0]->GetPosition(), GetSize().x).HitNum > 0) {
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
						bool X = FALSE, Y = FALSE, Z = FALSE;
						move = VGet(0, 0, 0);
						float dist = 0;
						float x = VDot(coll[1]->GetRight(), dir), y = VDot(coll[1]->GetUp(), dir), z = VDot(coll[1]->GetFront(), dir);
						dir.x = VDot(VSub(GetPosition(), mcoll->GetPosition()), coll[1]->GetRight());
						dir.y = VDot(VSub(GetPosition(), mcoll->GetPosition()), coll[1]->GetUp());
						dir.z = VDot(VSub(GetPosition(), mcoll->GetPosition()), coll[1]->GetFront());
						if (x < 0) { x *= -1; } if (y < 0) { y *= -1; } if (z < 0) { z *= -1; }

						if (x > coll[1]->GetSize().x * 200) { X = TRUE; }
						if (y > coll[1]->GetSize().y * 200) { Y = TRUE; }
						if (z > coll[1]->GetSize().z * 200) { Z = TRUE; }

						if (X == TRUE) {
							dist = dir.x;
							float tmp = dir.x / VSize(VGet(dir.x , dir.y * Y , dir.z * Z));
							if (dist < 0) { dist = -GetSize().x * tmp - mcoll->GetSize().x * 200 - dist; }
							else { dist = GetSize().x * VDot(dir, coll[1]->GetRight()) + mcoll->GetSize().x * 200 - dist; }
							VECTOR m = VAdd(move, VScale(coll[1]->GetRight(), dist));
							move = VAdd(move, m);
						}
						if (Y == TRUE) {
							dist = dir.y;
							float tmp = dir.y / VSize(VGet(dir.x * X, dir.y, dir.z * Z));
							if (dist < 0) { dist = -GetSize().y * tmp - mcoll->GetSize().y * 200 - dist; }
							else { dist = GetSize().y * VDot(dir, coll[1]->GetUp()) + mcoll->GetSize().y * 200 - dist; }
							VECTOR m = VAdd(move, VScale(coll[1]->GetUp(), dist));
							move = VAdd(move, m);
						}
						if (Z == TRUE) {
							dist = dir.z;
							float tmp = dir.z / VSize(VGet(dir.x * X, dir.y * Y, dir.z));
							if (dist < 0) { dist = -GetSize().z * tmp - mcoll->GetSize().z * 200 - dist; }
							else { dist = GetSize().z * VDot(dir, coll[1]->GetFront()) + mcoll->GetSize().z * 200 - dist; }
							VECTOR m = VAdd(move, VScale(coll[1]->GetFront(), dist));
							move = VAdd(move, m);
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
		DrawSphere3D(GetPosition(), GetSize().x, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
		break;


	default:
		MV1DrawModel(Handle);
		break;

	}
}
