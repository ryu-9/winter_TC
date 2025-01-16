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
	for (auto mcoll : _Owner->GetMode()->GetMCollision()) {

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

				MoveComponent* MoveCom = _Owner->GetComponent<class MoveComponent>();

				typeSum += mcoll->GetType() * mcoll->GetType();
				MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(coll[1]->GetHandle(), -1, coll[0]->GetPosition(), coll[0]->GetSize().x);
				if (result.HitNum > 0) {

					num++;

					flag = TRUE;
					VECTOR position[3];
					int num[2] = { 0, 0 };

					for (int i = 0; i < result.HitNum; i++) {
						MV1_COLL_RESULT_POLY mesh = result.Dim[i];

						if (VSize(VSub(coll[0]->GetPosition(), position[2])) > VSize(VSub(coll[0]->GetPosition(), mesh.HitPosition))) {
							position[2] = mesh.HitPosition;
							if (VSize(VCross(VSub(mesh.Position[0], mesh.Position[1]), VSub(mesh.Position[0], mesh.HitPosition))) <= 0.01 ||
								VSize(VCross(VSub(mesh.Position[1], mesh.Position[2]), VSub(mesh.Position[1], mesh.HitPosition))) <= 0.01 ||
								VSize(VCross(VSub(mesh.Position[2], mesh.Position[0]), VSub(mesh.Position[2], mesh.HitPosition))) <= 0.01) {
								//position[2] = mesh.HitPosition;
							}
							else {
								//position[2] = mesh.HitPosition;
								//break;
							}
						}

					}


					num[0] = 0; num[1] = 0;
					position[1] = position[0];

					VECTOR oldPos = coll[0]->GetPosition();
					if (MoveCom != nullptr) {
						oldPos = MoveCom->GetOldPosition();
					}
					VECTOR tmp = VSub(MoveCom->GetOldPosition(), coll[0]->GetOwner()->GetPosition());
					MV1_COLL_RESULT_POLY_DIM Presult = MV1CollCheck_Sphere(coll[1]->GetHandle(), -1, oldPos, coll[0]->GetSize().x + VSize(tmp));

					if (Presult.HitNum > 0) {

						VECTOR move = VGet(0, 0, 0);
	

						for (int i = 0; i < Presult.HitNum; i++) {
							MV1_COLL_RESULT_POLY mesh = Presult.Dim[i];

							if (VSize(VSub(oldPos, position[0])) > VSize(VSub(oldPos, mesh.HitPosition))) {
								position[0] = mesh.HitPosition;
								float cross[3];
								cross[0] = VSize(VCross(VSub(mesh.Position[0], mesh.Position[1]), VSub(mesh.Position[0], mesh.HitPosition)));
								cross[1] = VSize(VCross(VSub(mesh.Position[1], mesh.Position[2]), VSub(mesh.Position[1], mesh.HitPosition)));
								cross[2] = VSize(VCross(VSub(mesh.Position[2], mesh.Position[0]), VSub(mesh.Position[2], mesh.HitPosition)));
								if (cross[0] <= 0.01 * VSize(VSub(mesh.Position[2], mesh.Position[0])) * VSize(VSub(mesh.Position[0], mesh.HitPosition)) ||
									cross[1] <= 0.01 * VSize(VSub(mesh.Position[0], mesh.Position[1])) * VSize(VSub(mesh.Position[1], mesh.HitPosition)) ||
									cross[2] <= 0.01 * VSize(VSub(mesh.Position[1], mesh.Position[2])) * VSize(VSub(mesh.Position[2], mesh.HitPosition))) {
									move = VGet(0, 0, 0);
									position[0] = mesh.HitPosition;
									shomen = FALSE;
								}
								else {
									position[0] = mesh.HitPosition;
									move = mesh.Normal;
									shomen = TRUE;
									//break;
								}
							}
						}


						if (shomen == FALSE) {
							move = VNorm(VSub(oldPos, position[0]));
						}
						float deg = 0;
						if (move.y >= cos(deg / 180 * atan(1) * 4)) {
							MoveComponent* SelfMC = _Owner->GetComponent<MoveComponent>();
							if (SelfMC != nullptr) {
								SelfMC->SetStand(TRUE);
							}
						}

						if (move.y <= -0.5) {
							int test = 0;
						}

						if (mcoll->isMove == TRUE) {
						}
						else {
							if (VSize(move) > 1.1) {
								int tset = 0;
							}
							//move = VNorm(move);
							float dot = VDot(move, VSub(position[2], coll[0]->GetPosition()));
							if (dot > 0) {
								if (shomen == FALSE) {
									int test = 0;
								}

							}
							float a = VSize(VSub(position[2], coll[0]->GetPosition()));
							float r = coll[0]->GetSize().x;
							float debugSQRT = 4 * (dot * dot) - 4 * (a * a - r * r);
							if (debugSQRT < 0) {
								int test = 0;
								debugSQRT = 0;
							}

							float tmp = (2 * dot + sqrt(debugSQRT)) / 2;



							_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(move, tmp)));
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
						}
					}

					MV1CollResultPolyDimTerminate(Presult);

					MV1SetPosition(Handle, GetPosition());
					MV1SetScale(Handle, GetSize());
					MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
					MV1RefreshCollInfo(Handle);
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
