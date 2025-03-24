#include "CameraActor.h"
#include "PlayerActor.h"
#include "ApplicationGlobal.h"


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
	switch (gGlobal._SelectStage) {
	case 0:
		EffectController::GetInstance()->AddEffect(new FogSpriteComponent(this, -1000000, GetColor(150, 150, 255), 1000, 6000));
		break;
	case 1:
		EffectController::GetInstance()->AddEffect(new FogSpriteComponent(this, -1000000, GetColor(255, 225, 150), 1000, 6000));
		break;
	case 2:
	case 3:
		EffectController::GetInstance()->AddEffect(new FogSpriteComponent(this, -1000000, GetColor(255, 150, 150), 1000, 6000));
		break;

	default:
		EffectController::GetInstance()->AddEffect(new FogSpriteComponent(this, -1000000, GetColor(125, 125, 225), 1000, 6000));
		break;
	}
		EffectController::GetInstance()->AddEffectFlag(_SkySphere->GetSprite(), "Fog", false);
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
	_Target = VGet(0, 0, 0);
	_Position = VGet(0, 0, 0);
	SetCameraPositionAndTarget_UpVecY(_cOwner->GetPosition(), _cOwner->GetDirection());
	EffectController::GetInstance()->AddEffect(new ShadowMapSpriteComponent(_Owner, 4096, VGet(0.75, -1, 0.75), VGet(0, 0, 0), 0, 800));
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
	angle = VGet(0, 200, -300);
	angle = VNorm(angle);

	angle = VGet(0, 200, -300);
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
		smap[0]->SetMinLength(VGet(-2 * _Dist, -2 * _Dist, -2 * _Dist));
		smap[0]->SetMaxLength(VGet(2 * _Dist, 2 * _Dist, _Dist * 6));
		smap[1]->SetTarget(_Player[0]->GetPosition());
		smap[1]->SetMinLength(VGet(-_Player[0]->GetSize().x * 200, -_Player[0]->GetPosition().y, -_Player[0]->GetSize().x * 200));
		smap[1]->SetMaxLength(VScale(_Player[0]->GetSize(), 200));
		smap[2]->SetTarget(_Player[1]->GetPosition());
		smap[2]->SetMinLength(VGet(-_Player[1]->GetSize().x * 200, -_Player[1]->GetPosition().y, -_Player[1]->GetSize().x * 200));
		smap[2]->SetMaxLength(VScale(_Player[1]->GetSize(), 200));
	}

	float dt = GetOwner()->GetMode()->GetStepTm();
	VECTOR camtarget;
	if(_Easing[0] == 0) { _Easing[0] = 1; }	
	if (_Target2 == nullptr) {
		camtarget = VAdd(VScale(_Target, (float)_Easing[1] / _Easing[0]), VScale(v, (float)(_Easing[0] - _Easing[1]) / _Easing[0]));
		_Easing[1] -= GetOwner()->GetMode()->GetStepTm();
		if (_Easing[1] < 0) { _Easing[1] = 0; }
		if (_Easing[0] > 500) {
			int tset = 0;
		}
	}
	else {
		camtarget = VAdd(VScale(*_Target2, (float)(_Easing[0] - _Easing[1])/_Easing[0]), VScale(v, (float)_Easing[1]/_Easing[0]));
		_Easing[1] -= GetOwner()->GetMode()->GetStepTm();
		if (_Easing[1] < 0) { _Easing[1] = 0; }
	}

	VECTOR campos;
	if (_Position2 == nullptr) {
		campos = VAdd(VScale(_Position, (float)_Easing[1] / _Easing[0]), VScale(VAdd(v, VScale(angle, _Dist)), (float)(_Easing[0] - _Easing[1]) / _Easing[0]));
		_Easing[1] -= GetOwner()->GetMode()->GetStepTm();
		if (_Easing[1] < 0) { _Easing[1] = 0; }
	}
	else {
		campos = VAdd(VScale(*_Position2, (float)(_Easing[0] - _Easing[1]) / _Easing[0]), VScale(VAdd(v, VScale(angle, _Dist)), (float)_Easing[1] / _Easing[0]));
		_Easing[1] -= GetOwner()->GetMode()->GetStepTm();
		if (_Easing[1] < 0) { _Easing[1] = 0; }
	}



	_Owner->SetPosition(camtarget);
	_Owner->SetDirection(campos);
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

void CameraComponent::SetPosition2(VECTOR* pos)
{
	_Position2 = pos;
	if (pos != nullptr) { 
		_Position = *_Position2;
	}
}

void CameraComponent::SetTarget2(VECTOR* target)
{
	_Target2 = target;
	if (target != nullptr) {
		_Target = *_Target2;
	}
}
