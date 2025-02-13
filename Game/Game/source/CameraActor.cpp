#include "CameraActor.h"
#include "PlayerActor.h"

CameraActor::CameraActor(ModeBase* mode)
	:ActorClass(mode)
{
	// カメラの設定（わかりやすい位置に）
	_Position = VGet(0, 90.f, -300.f);
	_Direction = VGet(0, 0, 0);
	_clipNear = 2.f;
	_clipFar = 200000.f;
	_Follow = nullptr;
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

	SetCameraPositionAndTarget_UpVecY(_cOwner->GetPosition(), _cOwner->GetDirection());
	EffectController::GetInstance()->AddShadowMap(2048, VGet(0.75, -1, 0.75), VGet(0, 0, 0), 0, 800);
	EffectController::GetInstance()->AddShadowMap(1024, VGet(0, -1, 0), VGet(0, 0, 0), 1, 800);
	EffectController::GetInstance()->AddShadowMap(1024, VGet(0, -1, 0), VGet(0, 0, 0), 2, 800);
	/*

	_ShadowMap[0] = new ShadowMapSpriteComponent(_Owner, 2048, VGet(0.75, -1, 0.75), VGet(0, 0, 0), 0, 800);
	_ShadowMap[1] = new ShadowMapSpriteComponent(_Owner, 1024, VGet(0, -1, 0), VGet(0, 0, 0), 1, 800);
	_ShadowMap[2] = new ShadowMapSpriteComponent(_Owner, 1024, VGet(0, -1, 0), VGet(0, 0, 0), 2, 800);
	*/

}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::ProcessInput()
{
	/*

	*/
	// アナログスティック対応

	DINPUT_JOYSTATE di;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &di);
	float lx, ly, rx, ry;	// 左右アナログスティックの座標
	float analogMin = 0.3f;	// アナログ閾値
	// Logicoolパッドの場合
	lx = (float)di.X / 1000.f; ly = (float)di.Y / 100.f;
	rx = (float)di.Z / 100.f; ry = -(float)di.Rz / 100.f;	// Y軸反転


	// Y軸回転
	float sx = _cOwner->GetPosition().x - _cOwner->GetDirection().x;
	float sz = _cOwner->GetPosition().z - _cOwner->GetDirection().z;
	float rad = atan2(sz, sx);
	float length = sqrt(sz * sz + sx * sx);
	if (rx > analogMin) { rad -= 0.05f; }
	if (rx < -analogMin) { rad += 0.05f; }
	_cOwner->SetPosition(VGet(_cOwner->GetDirection().x + cos(rad) * length, _cOwner->GetPosition().y, _cOwner->GetDirection().z + sin(rad) * length));
	// Y位置
	if (ry > analogMin) { _cOwner->SetPosition(VGet(_cOwner->GetPosition().x, _cOwner->GetPosition().y - 50.f, _cOwner->GetPosition().z)); }
	if (ry < -analogMin) { _cOwner->SetPosition(VGet(_cOwner->GetPosition().x, _cOwner->GetPosition().y + 50.f, _cOwner->GetPosition().z)); }

	// カメラ設定更新
	VECTOR pos[2] = { VGet(0,0,0),VGet(0,0,0) };
	if (_Player[0] != nullptr) {
		pos[0] = _Player[0]->GetPosition();
	}
	if (_Player[1] != nullptr) {
		pos[1] = _Player[1]->GetPosition();
	}
	VECTOR v = VAdd(pos[0], pos[1]);
	float dist = VSize(VSub(pos[0], pos[1])) + 100;
	VECTOR angle = VSub(GetCameraPosition(), GetCameraTarget());
	angle = VNorm(angle);

	angle = VTransform(angle, MGetRotY(rx/1000));
	float camrad = atan2(angle.y, VSize(VGet(angle.x, 0, angle.z)));
	camrad += ry / 1000;
	angle.y = sin(camrad);

	if (dist < 1000) { dist = 1000; }
	v = VScale(v, 0.5f);
	_Owner->SetDirection(v);
	EffectController::GetInstance()->GetShadowMap(0)->SetTarget(VAdd(v, VGet(0, -v.y - 100, 0)));
	EffectController::GetInstance()->GetShadowMap(0)->SetMinLength(VGet(-dist, -dist, -dist));
	EffectController::GetInstance()->GetShadowMap(0)->SetMaxLength(VGet(dist, dist, dist));
	EffectController::GetInstance()->GetShadowMap(1)->SetTarget(_Player[0]->GetPosition());
	EffectController::GetInstance()->GetShadowMap(1)->SetMinLength(VGet(-800, -_Player[0]->GetPosition().y, -800));
	EffectController::GetInstance()->GetShadowMap(2)->SetTarget(_Player[1]->GetPosition());
	EffectController::GetInstance()->GetShadowMap(2)->SetMinLength(VGet(-800, -_Player[1]->GetPosition().y, -800));

	/*
		_ShadowMap[0]->SetTarget(VAdd(v, VGet(0, -v.y - 100, 0)));
	_ShadowMap[0]->SetMinLength(VGet(-dist, -dist, -dist));
	_ShadowMap[0]->SetMaxLength(VGet(dist, dist, dist));
	_ShadowMap[1]->SetTarget(_Player[0]->GetPosition());
	_ShadowMap[1]->SetMinLength(VGet(-200, -_Player[0]->GetPosition().y, -200));
	_ShadowMap[2]->SetTarget(_Player[1]->GetPosition());
	_ShadowMap[2]->SetMinLength(VGet(-200, -_Player[1]->GetPosition().y, -200));
	*/


	_Owner->SetPosition(VAdd(v, VScale(angle, dist)));
	//SetCameraPositionAndTarget_UpVecY(VGet(v.x + dist, v.y + dist, v.z), v);
	SetCameraPositionAndTarget_UpVecY(_cOwner->GetPosition(), _cOwner->GetDirection());
	SetCameraNearFar(_cOwner->GetClipNear(), _cOwner->GetClipFar());


}

void CameraComponent::Update()
{

}

void CameraComponent::SetPlayer(PlayerActor* player1, PlayerActor* player2)
{
	_Player[0] = player1;
	_Player[1] = player2;
	EffectController* ec = EffectController::GetInstance();
	if (ec == nullptr) { return; }
	ec->GetShadowMap(1)->AddSprite(player1->GetComponent<SpriteComponent>());
	ec->GetShadowMap(0)->AddRemoveSprite(player1->GetComponent<SpriteComponent>());
	ec->GetShadowMap(2)->AddSprite(player2->GetComponent<SpriteComponent>());
	ec->GetShadowMap(0)->AddRemoveSprite(player2->GetComponent<SpriteComponent>());
	
	//_ShadowMap[1]->AddSprite(player1->GetComponent<SpriteComponent>());
	//_ShadowMap[2]->AddSprite(player2->GetComponent<SpriteComponent>());
}
