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
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	

	// カメラの向いている角度を取得
	float sx = _pOwner->GetMode()->GetCamera()->GetPosition().x - _pOwner->GetMode()->GetCamera()->GetDirection().x;
	float sz = _pOwner->GetMode()->GetCamera()->GetPosition().z - _pOwner->GetMode()->GetCamera()->GetDirection().z;
	float camrad = atan2(sz, sx);



	// 移動方向を決める
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




	switch (_pOwner->GetModeNum()) {
	case 0:		// 通常移動
	{
		// vをrad分回転させる
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
		if (_DashTime <= 0) {
			if (v.x != 0 || v.z != 0) {
				_DashDir = VNorm(v);
			}
		}
		else {
			_DashTime--;
			v = VScale(_DashDir, _DashTime);
		}

		// 移動前の位置を保存
		VECTOR oldvPos = _pOwner->GetPosition();
		VECTOR oldv = _pOwner->GetMove();


		if (_Owner->GetComponent<MoveCollisionComponent>()->GetFlag() == FALSE) {
			// 当たっていなかったら、カメラも移動する
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

		if (ApplicationMain::GetInstance()->GetTrg() & PAD_INPUT_4 && _DashTime <= 0) {
			_DashTime = 100;
		}
		if (ApplicationMain::GetInstance()->GetTrg() & PAD_INPUT_3) {
			v.y = 50;
			_DashTime = 0;
		}
		if (ApplicationMain::GetInstance()->GetTrg() & PAD_INPUT_2) {
			_pOwner->SetSize(VGet(0.5, 0.5, 0.5));
		}
		if (ApplicationMain::GetInstance()->GetKey() & PAD_INPUT_1) {
			v.y = 1;
		}

		// 移動量のセット
		float size = VSize(VSub(_pOwner->GetPosition(), GetOldPosition())) / 10000;
		//_pOwner->SetMove(v);
		SetVelocity(v);
		if (GetStand() == TRUE) {
			_pOwner->SetSize(VAdd(_pOwner->GetSize(), VGet(size, size, size)));
		}
	}
	
		break;

	case 1:		// 下半身の移動

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
		if (_DashTime <= 0) {
			if (v.x != 0 || v.z != 0) {
				_DashDir = VNorm(v);
			}
		}
		else {
			_DashTime--;
			v = VScale(_DashDir, _DashTime);
		}

		// 移動前の位置を保存
		VECTOR oldvPos = _pOwner->GetPosition();
		VECTOR oldv = _pOwner->GetMove();


		if (_Owner->GetComponent<MoveCollisionComponent>()->GetFlag() == FALSE) {
			// 当たっていなかったら、カメラも移動する
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

		if (ApplicationMain::GetInstance()->GetTrg() & PAD_INPUT_4 && _DashTime <= 0) {
			_DashTime = 100;
		}
		if (ApplicationMain::GetInstance()->GetTrg() & PAD_INPUT_3) {
			v.y = 50;
			_DashTime = 0;
		}
		if (ApplicationMain::GetInstance()->GetTrg() & PAD_INPUT_2) {
			_pOwner->SetSize(VGet(0.5, 0.5, 0.5));
		}
		if (ApplicationMain::GetInstance()->GetKey() & PAD_INPUT_1) {
			v.y = 1;
		}

		//_pOwner->SetMove(v);
		SetVelocity(v);


		}
		// vをrad分回転させる

	
		break;

	case 2:
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

