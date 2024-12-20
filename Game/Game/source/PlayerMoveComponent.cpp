#include "PlayerMoveComponent.h"
#include "ApplicationMain.h"
#include "PlayerActor.h"
#include "CameraActor.h"

PlayerMoveComponent::PlayerMoveComponent(PlayerActor* owner, int updateOrder)
	:MoveComponent(owner, updateOrder)
	,_pOwner(owner)
	,_colSubY(40.f)
{

}

PlayerMoveComponent::~PlayerMoveComponent()
{
}

void PlayerMoveComponent::ProcessInput()
{
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	

	// �J�����̌����Ă���p�x���擾
	float sx = _pOwner->GetMode()->GetCamera()->GetPosition().x - _pOwner->GetMode()->GetCamera()->GetDirection().x;
	float sz = _pOwner->GetMode()->GetCamera()->GetPosition().z - _pOwner->GetMode()->GetCamera()->GetDirection().z;
	float camrad = atan2(sz, sx);

	// �ړ����������߂�
	VECTOR v = { 0,0,0 };
	float mvSpeed = 12.f;
	DINPUT_JOYSTATE input;
	/*
	if (key & PAD_INPUT_DOWN) { v.x = 1; }
	if (key & PAD_INPUT_UP) { v.x = -1; }
	if (key & PAD_INPUT_LEFT) { v.z = -1; }
	if (key & PAD_INPUT_RIGHT) { v.z = 1; }
	*/
	GetJoypadDirectInputState(DX_INPUT_KEY_PAD1 , &input);
	v.x = (float)input.Y / 1000;
	v.z = (float)input.X / 1000;


	// v��rad����]������
	float length = 0.f;
	if (VSize(v) > 0.f) { 
		if (VSize(v) > 1) {
			length = mvSpeed;
		}
		else {
			length = mvSpeed * VSize(v);
		}
	}
	else {
	}
	float rad = atan2(v.z, v.x);
	v.x = cos(rad + camrad) * length;
	v.z = sin(rad + camrad) * length;

	// �ړ��O�̈ʒu��ۑ�
	VECTOR oldvPos = _pOwner->GetPosition();
	VECTOR oldv = _pOwner->GetMove();

	// �R���W��������ň��������������ɁAescapeTbl[]���Ɋp�x��ς��ĉ�������݂�
	float escapeTbl[] = {
		0, -10, 10, -20, 20, -30, 30, -40, 40, -50, 50, -60, 60, -70, 70, -80, 80,
	};
	for (int i = 0; i < sizeof(escapeTbl) / sizeof(escapeTbl[0]); i++) {
		// escapeTbl[i]�̕������ړ���v��]
		float escape_rad = DEG2RAD(escapeTbl[i]);
		v.x = cos(rad + camrad + escape_rad) * length;
		v.z = sin(rad + camrad + escape_rad) * length;

		// v�̕��ړ�
		//_pOwner->SetPosition(VAdd(_pOwner->GetPosition(), v));

		// �R���W�����������Ȃ��Ȃ烋�[�v���甲����
		if (!_pOwner->GetMode()->GetUseCollision()) {
			// �J�������ړ�����
			_pOwner->GetMode()->GetCamera()->SetPosition(VAdd(_pOwner->GetMode()->GetCamera()->GetPosition(), v));
			_pOwner->GetMode()->GetCamera()->SetDirection(VAdd(_pOwner->GetMode()->GetCamera()->GetDirection(), v));
			break;
		}

		// Stage�Ƃ̃R���W��������
		// �v���C���[�̈ʒu���擾
		VECTOR pos = _pOwner->GetPosition();
		
		// �v���C���[�̈ʒu�����ɁA�ǂ̃u���b�N�Ɠ������Ă��邩���v�Z
		int x = (int)((pos.x +200) / 400);
		int y = (int)((pos.z-200) / 400);

	
			// �������Ă��Ȃ�������A�J�������ړ�����
			_pOwner->GetMode()->GetCamera()->SetPosition(VAdd(_pOwner->GetMode()->GetCamera()->GetPosition(), v));
			_pOwner->GetMode()->GetCamera()->SetDirection(VAdd(_pOwner->GetMode()->GetCamera()->GetDirection(), v));
			break;
		



		
	}

	// �ړ��ʂ̃Z�b�g
	float size = VSize(v)/10000;
	//_pOwner->SetMove(v);
	SetVelocity(v);
	_pOwner->SetSize(VAdd(_pOwner->GetSize(), VGet(size, size, size)));

}

void PlayerMoveComponent::Receive(int message)
{

	switch (message)
	{
	case 2:
		return;
		break;
	default:
		break;
	}
}

