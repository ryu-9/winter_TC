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

				typeSum += Type * Type;
				if (typeSum < mcoll->GetType()* mcoll->GetType()) {
					coll[0] = this; coll[1] = mcoll;
				}
				else {
					coll[0] = mcoll; coll[1] = this;
				}
				typeSum += mcoll->GetType() * mcoll->GetType();

				MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(coll[1]->GetHandle(), -1, coll[0]->GetPosition(),coll[0]->GetSize().x);

				if (result.HitNum > 0) {

					VECTOR move = VGet(0, 0, 0);
					
					for (int i = 0; i < result.HitNum;i++) {
						MV1_COLL_RESULT_POLY mesh = result.Dim[i];
						if (mesh.PositionWeight[0] == 0 || mesh.PositionWeight[1] == 0 || mesh.PositionWeight[2] == 0) {
							move = VAdd(move, VGet(mesh.Position[0].x, mesh.Position[0].y, mesh.Position[0].z));
						}
						else {
							move = mesh.Normal;
							break;
						}
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
	if (isMove == TRUE) {
		DrawFormatString(0, 160, GetColor(255, 255, 255), "%f , %f", _Owner->GetPosition().x, _Owner->GetPosition().y);
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
