#include "CameraActor.h"

CameraActor::CameraActor(ModeBase* mode)
	:ActorClass(mode)
{
	// �J�����̐ݒ�i�킩��₷���ʒu�Ɂj
	_Position = VGet(0, 90.f, -300.f);
	_Direction = VGet(0, 60, 0);
	_clipNear = 2.f;
	_clipFar = 20000.f;
	new CameraComponent(this);
}

CameraActor::~CameraActor()
{
}

void CameraActor::UpdateActor()
{
}

CameraComponent::CameraComponent(CameraActor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, _cOwner(owner)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::ProcessInput()
{

	// �A�i���O�X�e�B�b�N�Ή�
	DINPUT_JOYSTATE di;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &di);
	float lx, ly, rx, ry;	// ���E�A�i���O�X�e�B�b�N�̍��W
	float analogMin = 0.3f;	// �A�i���O臒l
	// Logicool�p�b�h�̏ꍇ
	lx = (float)di.X / 1000.f; ly = (float)di.Y / 1000.f;
	rx = (float)di.Z / 1000.f; ry = -(float)di.Rz / 1000.f;	// Y�����]


	// Y����]
	float sx = _cOwner->GetPosition().x - _cOwner->GetDirection().x;
	float sz = _cOwner->GetPosition().z - _cOwner->GetDirection().z;
	float rad = atan2(sz, sx);
	float length = sqrt(sz * sz + sx * sx);
	if (rx > analogMin) { rad -= 0.05f; }
	if (rx < -analogMin) { rad += 0.05f; }
	_cOwner->SetPosition(VGet(_cOwner->GetDirection().x + cos(rad) * length,_cOwner->GetPosition().y, _cOwner->GetDirection().z + sin(rad) * length));
	// Y�ʒu
	if (ry > analogMin) { _cOwner->SetPosition(VGet(_cOwner->GetPosition().x, _cOwner->GetPosition().y - 20.f , _cOwner->GetPosition().z));}
	if (ry < -analogMin) { _cOwner->SetPosition(VGet(_cOwner->GetPosition().x, _cOwner->GetPosition().y + 20.f, _cOwner->GetPosition().z)); }
}

void CameraComponent::Update()
{
	// �J�����ݒ�X�V
	SetCameraPositionAndTarget_UpVecY(_cOwner->GetPosition(), _cOwner->GetDirection());
	SetCameraNearFar(_cOwner->GetClipNear(), _cOwner->GetClipFar());
}
