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

void MoveCollisionComponent::Update()
{
	MV1SetPosition(Handle, GetPosition());
	MV1SetScale(Handle, GetSize());
	MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
	MV1RefreshCollInfo(Handle);

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
				bool shomen = FALSE;
				MoveComponent* MoveCom = _Owner->GetComponent<class MoveComponent>();

				typeSum += Type * Type;
				if (typeSum < mcoll->GetType()* mcoll->GetType()) {
					coll[0] = this; coll[1] = mcoll;
				}
				else {
					coll[0] = mcoll; coll[1] = this;
				}
				typeSum += mcoll->GetType() * mcoll->GetType();
				MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(coll[1]->GetHandle(), -1, coll[0]->GetPosition(), coll[0]->GetSize().x);
				if (result.HitNum > 0) {
				
					VECTOR position[3];
					int num[2] = { 0, 0 };

					for (int i = 0; i < result.HitNum; i++) {
						MV1_COLL_RESULT_POLY mesh = result.Dim[i];
						if (VSize(VCross(VSub(mesh.Position[0], mesh.Position[1]), VSub(mesh.Position[0], mesh.HitPosition))) <= 0.01 ||
							VSize(VCross(VSub(mesh.Position[1], mesh.Position[2]), VSub(mesh.Position[1], mesh.HitPosition))) <= 0.01 ||
							VSize(VCross(VSub(mesh.Position[2], mesh.Position[0]), VSub(mesh.Position[2], mesh.HitPosition))) <= 0.01) {
							if (VEqual(position[2], mesh.HitPosition) == TRUE) {
								num[0]++;
							}
							else if (VEqual(position[1], mesh.HitPosition) == TRUE) {
								num[1]++;
							}
							else if (num[1] == 0) {
								position[1] = mesh.HitPosition;
								num[1]++;
							}
							if (num[0] < num[1]) {
								int tmp = num[0];
								VECTOR tmpPos = position[2];
								num[0] = num[1];
								position[2] = position[1];
								num[1] = tmp;
								position[1] = tmpPos;

							}
						}
						else {
							position[2] = mesh.HitPosition;
							break;
						}
					}
					if (num[0] == num[1]&&num[0]!=0) {
						position[2] = VScale(VAdd(position[1], position[2]), 0.5);
					}

					num[0] = 0; num[1] = 0;
					position[1] = position[0];

					VECTOR tmp = VSub(MoveCom->GetOldPosition(), coll[0]->GetOwner()->GetPosition());
					MV1_COLL_RESULT_POLY_DIM Presult = MV1CollCheck_Sphere(coll[1]->GetHandle(), -1, VAdd(coll[0]->GetPosition(), tmp), coll[0]->GetSize().x + VSize(tmp));

					if (Presult.HitNum > 0) {

						VECTOR move = VGet(0, 0, 0);

						for (int i = 0; i < Presult.HitNum; i++) {
							MV1_COLL_RESULT_POLY mesh = Presult.Dim[i];
							if (VSize(VCross(VSub(mesh.Position[0], mesh.Position[1]), VSub(mesh.Position[0], mesh.HitPosition))) <= 0.01 ||
								VSize(VCross(VSub(mesh.Position[1], mesh.Position[2]), VSub(mesh.Position[1], mesh.HitPosition))) <= 0.01 ||
								VSize(VCross(VSub(mesh.Position[2], mesh.Position[0]), VSub(mesh.Position[2], mesh.HitPosition))) <= 0.01) {
								if (VEqual(position[0], mesh.HitPosition) == TRUE) {
									num[0]++;
								}
								else if (VEqual(position[1], mesh.HitPosition) == TRUE) {
									num[1]++;
								}
								else if (num[1] == 0) {
									position[1] = mesh.HitPosition;
									num[1]++;
								}
								if (num[0] < num[1]) {
									int tmp = num[0];
									VECTOR tmpPos = position[0];
									num[0] = num[1];
									position[0] = position[1];
									num[1] = tmp;
									position[1] = tmpPos;

								}
							}
							else {
								move = VNorm(mesh.Normal);
								shomen = TRUE;
								position[0] = mesh.HitPosition;
								break;
							}
						}
						if (num[0] == num[1]) {
							if (VSize(VSub(position[0], coll[0]->GetPosition())) > VSize(VSub(position[2], coll[0]->GetPosition()))) {
								position[0] = position[1];
							}
						}

						if (shomen == FALSE) {
							move = VNorm(VSub(coll[0]->GetPosition(), position[0]));
						}

						if (move.y >= 0.05||move.y<-0.05) {
							MoveCom->SetStand(TRUE);
						}

						if (move.x >= 0.05 || move.x < -0.05) {
							int test = 0;
						}

						if (mcoll->isMove == TRUE) {
						}
						else {

							move = VNorm(move);
							float dot = VDot(move, VSub(position[2], GetPosition()));
							float a = VSize(VSub(position[2], GetPosition()));
							float r = coll[0]->GetSize().x;
							float debugSQRT = 4 * (dot * dot) - 4 * (a * a - r * r);
							if (debugSQRT < 0) {
								int test = 0;
								debugSQRT = 0;
							}

							float tmp = (2 * dot  + sqrt(debugSQRT)) / 2;

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
	if (isMove == TRUE) {
		DrawFormatString(0, 160, GetColor(255, 255, 255), "%f , %f, %f", _Owner->GetPosition().x, _Owner->GetPosition().y, _Owner->GetPosition().z);
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
