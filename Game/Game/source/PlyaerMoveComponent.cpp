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

	// 移動前の位置を保存
	VECTOR oldvPos = _pOwner->GetPosition();
	VECTOR oldv = _pOwner->GetMove();

	// コリジョン判定で引っかかった時に、escapeTbl[]順に角度を変えて回避を試みる
	float escapeTbl[] = {
		0, -10, 10, -20, 20, -30, 30, -40, 40, -50, 50, -60, 60, -70, 70, -80, 80,
	};
	for (int i = 0; i < sizeof(escapeTbl) / sizeof(escapeTbl[0]); i++) {
		// escapeTbl[i]の分だけ移動量v回転
		float escape_rad = DEG2RAD(escapeTbl[i]);
		v.x = cos(rad + camrad + escape_rad) * length;
		v.z = sin(rad + camrad + escape_rad) * length;

		// vの分移動
		//_pOwner->SetPosition(VAdd(_pOwner->GetPosition(), v));

		// コリジョン処理しないならループから抜ける
		if (!_pOwner->GetMode()->GetUseCollision()) {
			// カメラも移動する
			_pOwner->GetMode()->GetCamera()->SetPosition(VAdd(_pOwner->GetMode()->GetCamera()->GetPosition(), v));
			_pOwner->GetMode()->GetCamera()->SetDirection(VAdd(_pOwner->GetMode()->GetCamera()->GetDirection(), v));
			break;
		}

		// Stageとのコリジョン判定
		// プレイヤーの位置を取得
		VECTOR pos = _pOwner->GetPosition();
		
		// プレイヤーの位置を元に、どのブロックと当たっているかを計算
		int x = (int)((pos.x +200) / 400);
		int y = (int)((pos.z-200) / 400);

	
			// 当たっていなかったら、カメラも移動する
			_pOwner->GetMode()->GetCamera()->SetPosition(VAdd(_pOwner->GetMode()->GetCamera()->GetPosition(), v));
			_pOwner->GetMode()->GetCamera()->SetDirection(VAdd(_pOwner->GetMode()->GetCamera()->GetDirection(), v));
			break;
		



		
	}

	// 移動量のセット
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

