#include "CameraActor.h"
#include "PlayerActor.h"

CameraActor::CameraActor(ModeBase* mode)
	:ActorClass(mode)
{
	// カメラの設定（わかりやすい位置に）
	_Position = VGet(0, 200.f, -300.f);
	_Direction = VGet(0, 0, 0);
	_clipNear = 2.f;
	_clipFar = 200000.f;
	_Follow = nullptr;
	new CameraComponent(this);
	_SkySphere = new ModelComponent(this, "res/Stage/model/Dorm_Haikei.mv1");
	_SkySphere->SetIndipendent(true);
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
	EffectController::GetInstance()->AddEffect(new ShadowMapSpriteComponent(_Owner, 2048, VGet(0.75, -1, 0.75), VGet(0, 0, 0), 0, 800));
	EffectController::GetInstance()->AddEffect(new ShadowMapSpriteComponent(_Owner, 1024, VGet(0, -1, 0), VGet(0, 0, 0), 1, 800));
	EffectController::GetInstance()->AddEffect(new ShadowMapSpriteComponent(_Owner, 1024, VGet(0, -1, 0), VGet(0, 0, 0), 2, 800));
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
	//_cOwner->SetPosition(VGet(_cOwner->GetDirection().x + cos(rad) * length, _cOwner->GetPosition().y, _cOwner->GetDirection().z + sin(rad) * length));
	// Y位置
	//if (ry > analogMin) { _cOwner->SetPosition(VGet(_cOwner->GetPosition().x, _cOwner->GetPosition().y - 50.f, _cOwner->GetPosition().z)); }
	//if (ry < -analogMin) { _cOwner->SetPosition(VGet(_cOwner->GetPosition().x, _cOwner->GetPosition().y + 50.f, _cOwner->GetPosition().z)); }

	// カメラ設定更新
	VECTOR pos[2] = { VGet(0,0,0),VGet(0,0,0) };
	if (_Player[0] != nullptr) {
		pos[0] = _Player[0]->GetPosition();
	}
	if (_Player[1] != nullptr) {
		pos[1] = _Player[1]->GetPosition();
	}
	VECTOR v = VAdd(pos[0], pos[1]);
	_Dist = VSize(VSub(pos[0], pos[1])) + 100;
	VECTOR angle = VSub(GetCameraPosition(), GetCameraTarget());
	angle = VNorm(angle);

	angle = VTransform(angle, MGetRotY(rx/1000));
	float camrad = atan2(angle.y, VSize(VGet(angle.x, 0, angle.z)));
	camrad += ry / 1000;
	angle.y = sin(camrad);

	if (_Dist < 1000) { _Dist = 1000; }
	if (_Dist > 10000) { _Dist = 10000; }
	v = VScale(v, 0.5f);

	auto smap = EffectController::GetInstance()->GetEffect<ShadowMapSpriteComponent>();
	if (smap.size() >= 3) {
		smap[0]->SetTarget(VGet(v.x, -100, v.z));
		smap[0]->SetMinLength(VGet(-_Dist, -_Dist, 0));
		smap[0]->SetMaxLength(VGet(_Dist, _Dist, _Dist * 4));
		smap[1]->SetTarget(_Player[0]->GetPosition());
		smap[1]->SetMinLength(VGet(-_Player[0]->GetSize().x * 200, -_Player[0]->GetPosition().y, -_Player[0]->GetSize().x * 200));
		smap[1]->SetMaxLength(VScale(_Player[0]->GetSize(), 200));
		smap[2]->SetTarget(_Player[1]->GetPosition());
		smap[2]->SetMinLength(VGet(-_Player[1]->GetSize().x * 200, -_Player[1]->GetPosition().y, -_Player[1]->GetSize().x * 200));
		smap[2]->SetMaxLength(VScale(_Player[1]->GetSize(), 200));
	}


	/*
		_ShadowMap[0]->SetTarget(VAdd(v, VGet(0, -v.y - 100, 0)));
	_ShadowMap[0]->SetMinLength(VGet(-dist, -dist, -dist));
	_ShadowMap[0]->SetMaxLength(VGet(dist, dist, dist));
	_ShadowMap[1]->SetTarget(_Player[0]->GetPosition());
	_ShadowMap[1]->SetMinLength(VGet(-200, -_Player[0]->GetPosition().y, -200));
	_ShadowMap[2]->SetTarget(_Player[1]->GetPosition());
	_ShadowMap[2]->SetMinLength(VGet(-200, -_Player[1]->GetPosition().y, -200));
	*/
	//auto cam = dynamic_cast<CameraActor*>(_Owner);
	//cam->SetTarget(VAdd(v, VScale(angle, dist)));
	_Owner->SetPosition(v);
	_Owner->SetDirection(VAdd(v, VScale(angle, _Dist)));
	_Owner->SetSize(VScale(VGet(0.1, 0.1, 0.1), _Dist));
	//SetCameraPositionAndTarget_UpVecY(VGet(v.x + dist, v.y + dist, v.z), v);
	SetCameraPositionAndTarget_UpVecY(_Owner->GetDirection(), _cOwner->GetPosition());
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
	auto smap = ec->GetEffect<ShadowMapSpriteComponent>();
	if (smap.size() < 3) { return; }
	smap[1]->AddSprite(player1->GetComponent<SpriteComponent>()[0]);
	smap[0]->AddRemoveSprite(player1->GetComponent<SpriteComponent>()[0]);
	smap[2]->AddSprite(player2->GetComponent<SpriteComponent>()[0]);
	smap[0]->AddRemoveSprite(player2->GetComponent<SpriteComponent>()[0]);
	
	//_ShadowMap[1]->AddSprite(player1->GetComponent<SpriteComponent>());
	//_ShadowMap[2]->AddSprite(player2->GetComponent<SpriteComponent>());
}
