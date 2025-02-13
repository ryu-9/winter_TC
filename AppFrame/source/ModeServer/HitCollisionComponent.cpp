#include "HitCollisionComponent.h"
#include "ActorClass.h"
#include "ModelComponent.h"
#include "../AppFrame/source/ModelServer/ModelServer.h"

HitCollisionComponent::HitCollisionComponent(class ActorClass* owner, ModelComponent* model, VECTOR pos, VECTOR size, int type, bool move, bool active, int handle)
	: Component(owner)
	, _Model(model), Pos(pos), Size(size), Type(type), isMove(move), isActive(active), Handle(handle)
	, Rot(VGet(0, 0, 0)), Front(VGet(0, 0, 1)), Up(VGet(0, 1, 0)), OldMove(VGet(0, 0, 0)), devpos(VGet(0, 0, 0))
	, flag(false), shomen(false)
{
	// ���[�h�ɂ��̃R���|�[�l���g��ǉ�
	_Owner->GetMode()->AddHCollision(this);


	OldPos = GetPosition();
	devpos = OldPos;
	// �^�C�v��2�ȉ��̏ꍇ�͏�������I��
	if (type <= 2) { return; }

	// ���f���R���|�[�l���g��擾���A�n���h����ݒ�
	ModelComponent* modelComp = model;
	if (modelComp == nullptr) {
		ModelComponent* modelComp = _Owner->GetComponent<ModelComponent>()[0];
	}

	if (Handle == -1 && modelComp != nullptr) {
		Handle = modelComp->GetHandle();
	}

	MV1SetPosition(Handle, GetPosition());
	MV1SetScale(Handle, GetSize());
	MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
	MV1SetupCollInfo(Handle);

}

HitCollisionComponent::~HitCollisionComponent()
{
	// ���[�h���炱�̃R���|�[�l���g��폜
	_Owner->GetMode()->RemoveHCollision(this);
}

std::deque<HitCollisionComponent*>& HitCollisionComponent::IsHit()
{
	MV1SetPosition(Handle, GetPosition());
	MV1SetScale(Handle, GetSize());
	MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
	MV1RefreshCollInfo(Handle);

	_IsHitList.clear();


	for (auto hcoll : _Owner->GetMode()->GetHCollision())
	{
		if (hcoll->GetIsActive() == TRUE)
		{
			if (hcoll->GetOwner() != _Owner)
			{
				if (HitCheck_Capsule_Capsule(GetPosition(), OldPos,GetSize().x, hcoll->GetPosition(), hcoll->GetOldPosition(), hcoll->GetSize().x)) {
					_IsHitList.insert(_IsHitList.begin(), hcoll);
				}

			}
		}
	}

	devpos = OldPos;
	OldPos = GetPosition();
	return _IsHitList;
	// TODO: return ステートメントをここに挿入します
}

VECTOR HitCollisionComponent::GetPosition()
{
	return VECTOR();
}

VECTOR HitCollisionComponent::GetSize()
{
	return VECTOR();
}

VECTOR HitCollisionComponent::GetUp()
{
	return VECTOR();
}

VECTOR HitCollisionComponent::GetFront()
{
	return VECTOR();
}

VECTOR HitCollisionComponent::GetRight()
{
	return VECTOR();
}

void HitCollisionComponent::DebugDraw()
{
	DrawCapsule3D(devpos, GetPosition(), GetSize().x, 5, GetColor(0,0,255), 0, false);
}

void HitCollisionComponent::SetRotation(VECTOR rot)
{
}
