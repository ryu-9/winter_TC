#include "HitCollisionComponent.h"
#include "ActorClass.h"
#include "ModelComponent.h"
#include "../AppFrame/source/ModelServer/ModelServer.h"

HitCollisionComponent::HitCollisionComponent(class ActorClass* owner, ModelComponent* model, VECTOR pos, VECTOR size, int type, bool move, bool active, int handle, bool useownersize)
	: Component(owner)
	, _Model(model), Pos(pos), Size(size), Type(type), isMove(move), isActive(active), Handle(handle)
	, Rot(VGet(0, 0, 0)), Front(VGet(0, 0, 1)), Up(VGet(0, 1, 0)), OldMove(VGet(0, 0, 0)), devpos(VGet(0, 0, 0))
	, flag(false), shomen(false), _UseOwnerSize(useownersize)
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
		auto mc = _Owner->GetComponent<ModelComponent>();
		if (mc.size() > 0) {
			modelComp = mc[0];
		}
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

	if (!isActive) { return _IsHitList; }
	for (auto hcoll : _Owner->GetMode()->GetHCollision())
	{
		if (hcoll->GetIsActive() == TRUE)
		{
			if (hcoll->GetOwner() != _Owner)
			{
				if (Type <= 2) {
					if (hcoll->GetType() <= 2) {
						if (HitCheck_Capsule_Capsule(GetPosition(), OldPos, GetSize().x, hcoll->GetPosition(), hcoll->GetOldPosition(), hcoll->GetSize().x)) {
							_IsHitList.insert(_IsHitList.begin(), hcoll);
						}
					}
					else {
						auto m = MV1CollCheck_Capsule(hcoll->GetHandle(), -1, GetPosition(), GetOldPosition(), GetSize().x);
						if (m.HitNum > 0) {
							_IsHitList.insert(_IsHitList.begin(), hcoll);
						}
						MV1CollResultPolyDimTerminate(m);
					}

				}
				else {
					auto m = MV1CollCheck_Capsule(Handle, -1, hcoll->GetPosition(), hcoll->GetOldPosition(), hcoll->GetSize().x);
					if (m.HitNum > 0) {
						_IsHitList.insert(_IsHitList.begin(), hcoll);
					}
					MV1CollResultPolyDimTerminate(m);
				}

			}
		}
	}

	return _IsHitList;
}

void HitCollisionComponent::Update()
{
	OldPos = devpos;
	devpos = GetPosition();
}

VECTOR HitCollisionComponent::GetPosition() {
	if (_UseOwnerSize) {
		return VAdd(_Owner->GetPosition(), VMulti(Pos, _Owner->GetSize()));
	} else {
		return VAdd(_Owner->GetPosition(), Pos);
	}
}

VECTOR HitCollisionComponent::GetSize() {
	if (_UseOwnerSize) {
		return VMulti(_Owner->GetSize(), Size);
	} else {
		return Size;
	}
}

VECTOR HitCollisionComponent::GetUp()
{
	if (Type <= 2 || _Model == nullptr) {
		return VGet(0, 1, 0);
	}
	return _Model->GetUp();
}

VECTOR HitCollisionComponent::GetFront()
{
	if (Type <= 2 || _Model == nullptr) {
		return VGet(0, 0, 1);
	}
	return _Model->GetFront();
}

VECTOR HitCollisionComponent::GetRight()
{
	return VCross(GetUp(), GetFront());
}

void HitCollisionComponent::DebugDraw()
{
	if (Type <= 2) {
		DrawCapsule3D(OldPos, GetPosition(), GetSize().x, 5, GetColor(0, 0, 255), 0, false);
	}
	else {
		VECTOR pos = GetPosition();
		MV1SetPosition(Handle, GetPosition());
		VECTOR size = GetSize();
		MV1SetScale(Handle, GetSize());
		VECTOR front = GetFront();
		VECTOR up = GetUp();
		MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
		MV1DrawModel(Handle);
	}

}

void HitCollisionComponent::SetRotation(VECTOR rot)
{
}

void HitCollisionComponent::RefleshCollInfo()
{
	VECTOR pos = GetPosition();
	MV1SetPosition(Handle, GetPosition());
	VECTOR size = GetSize();
	MV1SetScale(Handle, GetSize());
	MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
	MV1SetupCollInfo(Handle);
}

