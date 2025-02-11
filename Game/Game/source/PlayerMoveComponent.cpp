#include "PlayerMoveComponent.h"
#include "ApplicationMain.h"
#include "PlayerActor.h"
#include "CameraActor.h"

PlayerMoveComponent::PlayerMoveComponent(PlayerActor* owner, int updateOrder)
	:MoveComponent(owner, updateOrder)
	,_pOwner(owner)
	,_colSubY(40.f)
	,_DashTime(0)
	,_DashDir(VGet(0,0,1))
{

}

PlayerMoveComponent::~PlayerMoveComponent()
{
}

void PlayerMoveComponent::ProcessInput()
{
	int pn = _pOwner->GetPlayerNo();
	int key = ApplicationMain::GetInstance()->GetKey(pn);
	int trg = ApplicationMain::GetInstance()->GetTrg(pn);

	

	// �J�����̌����Ă���p�x��擾
	float sx = _pOwner->GetMode()->GetCamera()->GetPosition().x - _pOwner->GetMode()->GetCamera()->GetDirection().x;
	float sz = _pOwner->GetMode()->GetCamera()->GetPosition().z - _pOwner->GetMode()->GetCamera()->GetDirection().z;
	float camrad = atan2(sz, sx);



	// �ړ���������߂�
	VECTOR v = { 0,0,0 };
	float mvSpeed = 6.f;
	DINPUT_JOYSTATE input;
	/*
	if (key & PAD_INPUT_DOWN) { v.x = 1; }
	if (key & PAD_INPUT_UP) { v.x = -1; }
	if (key & PAD_INPUT_LEFT) { v.z = -1; }
	if (key & PAD_INPUT_RIGHT) { v.z = 1; }
	*/
	GetJoypadDirectInputState(pn, &input);
	v.x = (float)input.Y / 1000;
	v.z = (float)input.X / 1000;

	VECTOR velocity = GetVelocity();


	switch (_pOwner->GetModeNum()) {
	case 0:		// �ʏ�ړ�
	{
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
		if (_DashTime <= mvSpeed) {
			if (v.x != 0 || v.z != 0) {
				_DashDir = VNorm(v);
			}
		}
		else {
			_DashTime--;
			v = VScale(_DashDir, _DashTime);
		}

		{
			int dt = FpsController::GetInstance()->GetDeltaTime();
			if (v.x > 0) {
				if (v.x > velocity.x) {
					velocity.x += v.x / 100 * dt;
				}
			}
			else {
				if (v.x < velocity.x) {
					velocity.x += v.x / 100 * dt;
				}
			}
			if (v.z > 0) {
				if (v.z > velocity.z) {
					velocity.z += v.z / 100 * dt;
				}
			}
			else {
				if (v.z < velocity.z) {
					velocity.z += v.z / 100 * dt;
				}
			}
		
		}



		if (trg & PAD_INPUT_4 && _DashTime <= mvSpeed) {
			_DashTime = 100;
		}
		if (trg & PAD_INPUT_3) {
			velocity.y = 10;
			_DashTime = 0;
		}
		if (trg & PAD_INPUT_2) {
			_pOwner->SetSize(VGet(0.1, 0.1, 0.1));
		}
		if (trg & PAD_INPUT_1) {
			_pOwner->SetSize(VScale(_pOwner->GetSize(),2));
		}

		SetVelocity(velocity);
	}
	
		break;

	case 1:		// �����g�̈ړ�
	{

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
		if (_DashTime <= mvSpeed) {
			if (v.x != 0 || v.z != 0) {
				_DashDir = VNorm(v);
			}
		}
		else {
			_DashTime-=5;
			v = VScale(_DashDir, _DashTime);
		}

		// �ړ��O�̈ʒu��ۑ�
		VECTOR oldvPos = _pOwner->GetPosition();


		if (_Owner->GetComponent<MoveCollisionComponent>()->GetFlag() == FALSE) {
			// �������Ă��Ȃ�������A�J������ړ�����
			//_pOwner->GetMode()->GetCamera()->SetPosition(VAdd(_pOwner->GetMode()->GetCamera()->GetPosition(), v));
			//_pOwner->GetMode()->GetCamera()->SetDirection(VAdd(_pOwner->GetMode()->GetCamera()->GetDirection(), v));

		}
		{
			VECTOR old = GetOldPosition();
			VECTOR vector = VSub(_pOwner->GetPosition(), old);
			VECTOR dist = VSub(_pOwner->GetMode()->GetCamera()->GetPosition(), _pOwner->GetMode()->GetCamera()->GetDirection());
			_pOwner->GetMode()->GetCamera()->SetDirection(VAdd(_pOwner->GetMode()->GetCamera()->GetDirection(), vector));
			float height = dist.y;
			dist.y = 0;
			dist = VScale(VNorm(dist), VSize(_pOwner->GetSize()) * 200);
			dist = VAdd(_pOwner->GetMode()->GetCamera()->GetDirection(), dist);
			VECTOR campos = VGet(dist.x, dist.y + height, dist.z);
			_pOwner->GetMode()->GetCamera()->SetPosition(campos);
		}

		v.y = GetVelocity().y;

		if (trg & PAD_INPUT_4 && _DashTime <= 0) {
			_DashTime = 100;
		}
		if (trg & PAD_INPUT_3) {
			v.y = 50;
			_DashTime = 0;
		}
		if (trg & PAD_INPUT_2) {
			//_pOwner->SetSize(VGet(0.5, 0.5, 0.5));
		}
		if (key & PAD_INPUT_1) {
			v.y = 1;
		}

		//_pOwner->SetMove(v);
		SetVelocity(v);


		}
		// v��rad����]������

	
		break;

	case 2:

		_pOwner->GetInput()->SetVelocity(_pOwner->GetFriend()->GetInput()->GetVelocity());
		VECTOR v = _pOwner->GetFriend()->GetPosition();
		_pOwner->SetPosition(VGet(v.x, v.y + (_pOwner->GetFriend()->GetSize().y + _pOwner -> GetFriend()->GetSize().y) * 90, v.z));
		

		break;
	}


	//

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

