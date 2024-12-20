#include "ModelComponent.h"
#include "ActorClass.h"
#include "../ModelServer/ModelServer.h"

ModelComponent::ModelComponent(ActorClass* owner)
	:Component(owner)
	// �Đ����Ԃ̏�����
	,_TotalTime(0.f)
	,_PlayTime(0.0f)
	,_AttachIndex(-1)
{
	// ���f���f�[�^�̃��[�h�i�e�N�X�`�����ǂݍ��܂��j
	//_Handle = MV1LoadModel("res/Debug/chinpo.mv1");
	_Handle = ModelServer::GetInstance()->Add("res/Debug/chinpo.mv1");
}

ModelComponent::~ModelComponent()
{
}

void ModelComponent::ProcessInput()
{

}

void ModelComponent::Update()
{
	// move�̕��ړ�
//	_pOwner->SetPosition(VAdd(_pOwner->GetPosition(), _pOwner->GetMove()));
	/*
	// �ړ��ʂ����̂܂܃L�����̌����ɂ���
	if (VSize(_pOwner->GetMove()) > 0.f) {		// �ړ����Ă��Ȃ����͖������邽��
		_pOwner->SetDirection(_pOwner->GetMove());
		_pOwner->SetStatus(PlayerActor::STATUS::WALK);
	}
	else {
		_pOwner->SetStatus(PlayerActor::STATUS::WAIT);
	}

	// �X�e�[�^�X���ς���Ă��Ȃ����H
	if (_pOwner->IsChangeStatus()) {
		// �A�j���[�V�������A�^�b�`����Ă�����A�f�^�b�`����
		if (_AttachIndex != -1) {
			MV1DetachAnim(_Handle, _AttachIndex);
			_AttachIndex = -1;
		}
		// �X�e�[�^�X�ɍ��킹�ăA�j���[�V�����̃A�^�b�`
		switch (_pOwner->GetStatus()) {
		case PlayerActor::STATUS::WAIT:
			_AttachIndex = MV1AttachAnim(_Handle, MV1GetAnimIndex(_Handle, "idle"), -1, FALSE);
			break;
		case PlayerActor::STATUS::WALK:
			_AttachIndex = MV1AttachAnim(_Handle, MV1GetAnimIndex(_Handle, "run"), -1, FALSE);
			break;
		}
		// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
		_TotalTime = MV1GetAttachAnimTotalTime(_Handle, _AttachIndex);
		// �Đ����Ԃ�������
		_PlayTime = 0.0f;
	}
	else {
		// �Đ����Ԃ�i�߂�
		_PlayTime += 0.5f;
		if (VSize(_pOwner->GetMove()) > 6.f) { _PlayTime += 0.3f; }
	}

	// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
	if (_PlayTime >= _TotalTime) {
		_PlayTime = 0.0f;
	}
	*/
	
}

void ModelComponent::SetModelInfo()
{
	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(_Handle, _AttachIndex, _PlayTime);
	// �ʒu
	MV1SetPosition(_Handle, VAdd(_Owner->GetPosition(),VGet(0,_Owner->GetSize().y * 100, 0)));
	// ��������Y����]���Z�o
	VECTOR vRot = { 0,0,0 };
	vRot.y = atan2(_Owner->GetDirection().x * -1, _Owner->GetDirection().z * -1);		// ���f�����W���łǂ���������Ă��邩�Ŏ����ς��(�����-z�������Ă���ꍇ)
	//MV1SetRotationXYZ(_Handle, vRot);
	MV1SetScale(_Handle, _Owner->GetSize());
}


ModelSpriteComponent::ModelSpriteComponent(ActorClass* owner,ModelComponent* model, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	,_Model(model)
{
}

ModelSpriteComponent::~ModelSpriteComponent()
{
}

void ModelSpriteComponent::Draw()
{
	

	// ���x���擾
	VECTOR v =_Owner -> GetMove();
	// �p�x���擾
	VECTOR rot = _Owner->GetRotation();
	VECTOR rot2 = _Owner->GetRotation2();

	//	���x����p�x���Z�o
	rot = VTransform(rot, MGetRotX(v.z/100 / _Owner->GetSize().x / 2));
	rot = VTransform(rot, MGetRotZ(-v.x/100 / _Owner->GetSize().x / 2));
	rot2 = VTransform(rot2, MGetRotX(v.z / 100 / _Owner->GetSize().x / 2));
	rot2 = VTransform(rot2, MGetRotZ(-v.x / 100 / _Owner->GetSize().x / 2));

	//	�p�x���Z�b�g

	MV1SetRotationZYAxis(_Model->GetHandle(),rot,rot2,0);

	_Owner->SetRotation(rot);
	_Owner->SetRotation2(rot2);

	// ���f����`�悷�� ���f���R���|�ł悭�Ȃ����c�H
	{
		_Model->SetModelInfo();
		// �`��
		MV1DrawModel(_Model->GetHandle());
		DrawFormatString(0,0,GetColor(255,255,255),"x %f, y %f, z %f",rot.x,rot.y,rot.z);
	}
}

void ModelSpriteComponent::SetImage()
{
}

