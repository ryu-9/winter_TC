#include "MoveCollisionComponent.h"
#include "ActorClass.h"
#include "ModelComponent.h"
#include "MoveComponent.h"
#include "../AppFrame/source/ModelServer/ModelServer.h"

MoveCollisionComponent::MoveCollisionComponent(class ActorClass* owner,ModelComponent* model,  VECTOR pos, VECTOR size, int type, bool move, bool active, int handle)
	:Component(owner)
	,_Model(model), Pos(pos), Size(size), Type(type), isMove(move), isActive(active), Handle(handle)
	, Rot(VGet(0, 0, 0)), Front(VGet(0, 0, 1)), Up(VGet(0, 1, 0))
{
	_Owner->GetMode()->AddMCollision(this);

	if (isMove == TRUE) {
		MoveComponent* moveComp = _Owner->GetComponent<MoveComponent>();
	}


	if (type <= 2) { return; }
	
	ModelComponent* modelComp = _Owner->GetComponent<ModelComponent>();
	if(Handle==-1&&modelComp!=nullptr){
		Handle = modelComp->GetHandle();
	}
	//*/
	MV1SetPosition(Handle,GetPosition());
	MV1SetScale(Handle, GetSize());
	MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
	MV1SetupCollInfo(Handle);

}


MoveCollisionComponent::~MoveCollisionComponent()
{
_Owner->GetMode()->RemoveMCollision(this);
}

void MoveCollisionComponent::Update() {
	MV1SetPosition(Handle, GetPosition());
	MV1SetScale(Handle, GetSize());
	MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
	MV1RefreshCollInfo(Handle);

	int num = 0;
	
	if (isActive == FALSE || isMove == FALSE) {
		return;
	}
	flag = FALSE;
	shomen = FALSE;
	OldMove = VGet(0, 0, 0);
	for (auto mcoll : _Owner->GetMode()->GetMCollision()) {
		VECTOR oldmove = VGet(0, 0, 0);
		if (mcoll->GetIsActive() == TRUE) {
			if (mcoll != this) {

				int typeSum = 0;
				VECTOR move = VGet(0, 0, 0);
				MoveCollisionComponent* coll[2];

				typeSum += Type * Type;
				if (typeSum < mcoll->GetType() * mcoll->GetType()) {
					coll[0] = this; coll[1] = mcoll;
				}
				else {
					coll[0] = mcoll; coll[1] = this;
				}

				MoveComponent* MoveCom = coll[0]->GetOwner()->GetComponent<class MoveComponent>();

				VECTOR oldPos = coll[0]->GetPosition();
				
				bool moveflag = FALSE;
					if (MoveCom != nullptr) {
						oldPos = MoveCom->GetOldPosition();
						moveflag = TRUE;
					}
				VECTOR tmp = VSub(coll[0]->GetOwner()->GetPosition(), oldPos);

				typeSum += mcoll->GetType() * mcoll->GetType();
				MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(coll[1]->GetHandle(), -1, coll[0]->GetPosition(), oldPos, coll[0]->GetSize().x);
				if (result.HitNum > 0) {

	 				num++;

					flag = TRUE;
					std::deque <MV1_COLL_RESULT_POLY> polyList;


					for (int i = 0; i < result.HitNum; i++) {
						if (moveflag == FALSE || VDot(tmp, result.Dim[i].Normal) < 0) {
							polyList.push_back(result.Dim[i]);
						}
					}
					MV1_COLL_RESULT_POLY_DIM Presult = MV1CollCheck_Sphere(coll[1]->GetHandle(), -1, oldPos, coll[0]->GetSize().x);

					for (int i = 0; i < polyList.size(); i++) {
						for (int j = 0; j < Presult.HitNum; j++) {
							if (VEqual(polyList[i].Position[0], Presult.Dim[j].Position[0]) == TRUE&&
								VEqual(polyList[i].Position[1], Presult.Dim[j].Position[1]) == TRUE&&
								VEqual(polyList[i].Position[2], Presult.Dim[j].Position[2]) == TRUE) {
								move = VNorm(VSub(oldPos, Presult.Dim[j].HitPosition));
								move = VSub(move, VScale(move, VDot(move, OldMove)));
								if (VSize(move) != 0) {
									move = VNorm(move);
									tmp = VSub(coll[0]->GetOwner()->GetPosition(), oldPos);
									float length = 0;
									if (VEqual(move, Presult.Dim[j].Normal) == TRUE) {
										float a = VDot(tmp, move);
										if (a < 0) { a *= -1; }
										float b = VSize(VSub(Presult.Dim[j].HitPosition, oldPos));
										if (b < 0) { b *= -1; }
										float c = coll[0]->GetSize().x;;
										length = a - b + c;
									}
									else {
									
									
									}
									_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(move, length)));
									OldMove = move;
									MoveComponent* EnMoveCon = mcoll->GetOwner()->GetComponent<class MoveComponent>();
									VECTOR EnMove;
									if (EnMoveCon != nullptr) {
										EnMove = EnMoveCon->GetVelocity();
									}
									else { EnMove = VGet(0, 0, 0); }
									VECTOR velocity = VAdd(VSub(MoveCom->GetVelocity(), VScale(move, VDot(move, MoveCom->GetVelocity()))), VScale(move, VDot(move, EnMove)));
									if (velocity.y > 0.1) {
										int test = 0;
									}
									MoveCom->SetVelocity(velocity);

									float deg = 0;
									if (move.y >= cos(deg / 180 * atan(1) * 4)) {
										MoveComponent* SelfMC = _Owner->GetComponent<MoveComponent>();
										if (SelfMC != nullptr) {
											SelfMC->SetStand(TRUE);
										}
									}

									MV1SetPosition(Handle, GetPosition());
									MV1SetScale(Handle, GetSize());
									MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
									MV1RefreshCollInfo(Handle);

									break;
								}
								else if( j == Presult.HitNum -1){
									int test = 0;
								}

							}
						}
					}


					MV1CollResultPolyDimTerminate(Presult);

				}

				MV1CollResultPolyDimTerminate(result);

			}
		}

	}
	if (num > 2) {
		int test = 0;
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
	if (isMove == TRUE) {
		DrawFormatString(0, 160, GetColor(255, 255, 255), "%f , %f, %f", _Owner->GetPosition().x, _Owner->GetPosition().y, _Owner->GetPosition().z);
	}

	if (flag == TRUE) {
		DrawFormatString(0, 0, GetColor(255, 255, 255), "Hit");
	}
	if (shomen == TRUE) {
		DrawFormatString(0, 16, GetColor(255, 255, 255), "Shomen");
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

void MoveCollisionComponent::SetRotation(VECTOR rot)
{
	Rot = rot;
	VECTOR rotate = VAdd(_Model->GetRotation(), rot);
	MV1SetRotationXYZ(Handle, rotate);
	Front = VTransform(VGet(0, 0, 1), MGetRotX(rotate.x));
	Front = VTransform(Front, MGetRotY(rotate.y));
	Front = VTransform(Front, MGetRotZ(rotate.z));
	Up = VTransform(VGet(0, 1, 0), MGetRotX(rotate.x));
	Up = VTransform(Up, MGetRotY(rotate.y));
	Up = VTransform(Up, MGetRotZ(rotate.z));
}
