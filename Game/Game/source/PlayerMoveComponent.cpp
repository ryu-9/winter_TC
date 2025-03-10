#include "PlayerMoveComponent.h"
#include "ApplicationMain.h"
#include "PlayerActor.h"
#include "CameraActor.h"

PlayerMoveComponent::PlayerMoveComponent(PlayerActor* owner, int updateOrder)
	:MoveComponent(owner, 1.0, updateOrder)
	,_pOwner(owner)
	,_colSubY(40.f)
	,_DashTime(0)
	,_DashDir(VGet(0,0,1))
	, _DashFlag(false)
{

}

PlayerMoveComponent::~PlayerMoveComponent()
{
}

void PlayerMoveComponent::ProcessInput()
{
	int pn = _pOwner->GetPlayerNo();
	_Key = ApplicationMain::GetInstance()->GetKey(pn);
	_Trg = ApplicationMain::GetInstance()->GetTrg(pn);

	

	// �J�����̌����Ă���p�x��擾
	float sx = _pOwner->GetMode()->GetCamera()->GetPosition().x - _pOwner->GetMode()->GetCamera()->GetDirection().x;
	float sz = _pOwner->GetMode()->GetCamera()->GetPosition().z - _pOwner->GetMode()->GetCamera()->GetDirection().z;
	float camrad = atan2(sz, sx);



	// �ړ���������߂�
	VECTOR v = { 0,0,0 };
	float mvSpeed = 6.f;
	/*
	if (_Key & PAD_INPUT_DOWN) { v.x = 1; }
	if (_Key & PAD_INPUT_UP) { v.x = -1; }
	if (_Key & PAD_INPUT_LEFT) { v.z = -1; }
	if (_Key & PAD_INPUT_RIGHT) { v.z = 1; }
	*/
	GetJoypadDirectInputState(pn, &_Input);
	v.x = (float)_Input.Y / 1000;
	v.z = (float)_Input.X / 1000;

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
		v.x = -cos(rad + camrad) * length;
		v.z = -sin(rad + camrad) * length;
		if (_DashTime <= 0) {
			if (v.x != 0 || v.z != 0) {

				_DashDir =v;
				_DashDir.y = 0;
				_DashDir = VNorm(_DashDir);
			}
			if (_DashFlag) {
				_DashFlag = false;
				velocity = VGet(0, 0, 0);
			}
		}
		else {
			_DashFlag = true;
			velocity = VScale(_DashDir, 20);
		}
		_DashTime -= FpsController::GetInstance()->GetDeltaTime();

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



		if (GetStand()) {
			_JumpFlag = true;
			_pOwner->SetChangeFlag(false);
		}
		else if (!_pOwner->GetComponent<MoveCollisionComponent>()[0]->GetCollResult().size()) {
			_JumpFlag = false;
		}


		if (_Trg & PAD_INPUT_4 && _DashTime <= -1000 && _JumpFlag) {
			_DashTime = 500;
		}
		if (_Trg & PAD_INPUT_3) {
			_pOwner->SetChangeFlag(true);
			if (_JumpFlag) {
				velocity.y = 10;
				_JumpFlag = false;
			}
		}
		if (_Trg & PAD_INPUT_2) {
			_pOwner->SetSize(VGet(0.1, 0.1, 0.1));
		}
		if (_Trg & PAD_INPUT_1) {
			_pOwner->SetSize(VScale(_pOwner->GetSize(),2));
		}

		SetVelocity(velocity);
	}
	
		break;

	case 1:		// �����g�̈ړ�
	case 3:
	case 5:
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
		v.x = -cos(rad + camrad) * length;
		v.z = -sin(rad + camrad) * length;
		if (_DashTime <= 0) {
			if (v.x != 0 || v.z != 0) {
				_DashDir = VNorm(v);
			}
		}
		else {
			_DashTime-=FpsController::GetInstance()->GetDeltaTime();
			v = VScale(_DashDir, 5);
		}

		// �ړ��O�̈ʒu��ۑ�
		VECTOR oldvPos = _pOwner->GetPosition();


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

		v = VScale(v, _Owner->GetSize().x / _pOwner->GetFriend()->GetSize().x);
		v.y = GetVelocity().y;

		if (_Trg & PAD_INPUT_4 && _DashTime <= 0) {
			_DashTime = 200;
		}
		if (_Trg & PAD_INPUT_3) {
			v.y = 10;
			_DashTime = 0;
		}
		if (_Trg & PAD_INPUT_2) {
			//_pOwner->SetSize(VGet(0.5, 0.5, 0.5));
		}
		if (_Key & PAD_INPUT_1) {
			v.y = 1;
		}

		//_pOwner->SetMove(v);

		SetVelocity(v);


		}
		// v��rad����]������

	
		break;

	case 2:
	case 4:
	case 6:


		_pOwner->GetInput()->SetVelocity(_pOwner->GetFriend()->GetInput()->GetVelocity());
		VECTOR v = _pOwner->GetFriend()->GetPosition();
		_pOwner->SetPosition(VAdd(VGet(v.x, v.y + (_pOwner->GetFriend()->GetSize().y + _pOwner -> GetFriend()->GetSize().y) * 40, v.z), VScale( VGet(0, 160, 0), _pOwner->GetSize().x)));
		
		if (_Trg & PAD_INPUT_3) {
			_pOwner->ChangeAnim(3 + _pOwner->GetModeNum() / 2);
		}

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

