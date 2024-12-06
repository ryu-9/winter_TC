#include "ModelComponent.h"

ModelComponent::ModelComponent(PlayerActor* owner)
	:Component(owner)
	// �Đ����Ԃ̏�����
	,_pOwner(owner)
	,_TotalTime(0.f)
	,_PlayTime(0.0f)
	,_AttachIndex(-1)
{
	// ���f���f�[�^�̃��[�h�i�e�N�X�`�����ǂݍ��܂��j
	_Handle = MV1LoadModel("res/SDChar/SDChar.mv1");
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

	
}

void ModelComponent::SetModelInfo()
{
	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(_Handle, _AttachIndex, _PlayTime);
	// �ʒu
	MV1SetPosition(_Handle, _pOwner->GetPosition());
	// ��������Y����]���Z�o
	VECTOR vRot = { 0,0,0 };
	vRot.y = atan2(_pOwner->GetDirection().x * -1, _pOwner->GetDirection().z * -1);		// ���f�����W���łǂ���������Ă��邩�Ŏ����ς��(�����-z�������Ă���ꍇ)
	MV1SetRotationXYZ(_Handle, vRot);
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
	

	// ���f����`�悷�� ���f���R���|�ł悭�Ȃ����c�H
	{
		_Model->SetModelInfo();
		// �`��
		MV1DrawModel(_Model->GetHandle());
	}
}

void ModelSpriteComponent::SetImage()
{
}

