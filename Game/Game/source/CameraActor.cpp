#include "CameraActor.h"
#include "PlayerActor.h"

CameraActor::CameraActor(ModeBase* mode)
	:ActorClass(mode)
{
	// カメラの設定（わかりやすい位置に）
	_Position = VGet(0, 90.f, -300.f);
	_Direction = VGet(0, 60, 0);
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
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::ProcessInput()
{
	/*
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
	_cOwner->SetPosition(VGet(_cOwner->GetDirection().x + cos(rad) * length,_cOwner->GetPosition().y, _cOwner->GetDirection().z + sin(rad) * length));
	// Y位置
	if (ry > analogMin) { _cOwner->SetPosition(VGet(_cOwner->GetPosition().x, _cOwner->GetPosition().y - 50.f , _cOwner->GetPosition().z));}
	if (ry < -analogMin) { _cOwner->SetPosition(VGet(_cOwner->GetPosition().x, _cOwner->GetPosition().y + 50.f, _cOwner->GetPosition().z)); }

	*/
	// アナログスティック対応

	

}

void CameraComponent::Update()
{
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
	if (dist < 1000) { dist = 1000; }
	v = VScale(v, 0.5f);
	_Owner->SetDirection(v);
	_Owner->SetPosition(VGet(v.x + dist, v.y + dist, v.z));
	SetCameraPositionAndTarget_UpVecY(VGet(v.x + dist, v.y + dist, v.z), v);
	//SetCameraPositionAndTarget_UpVecY(_cOwner->GetPosition(), _cOwner->GetDirection());
	SetCameraNearFar(_cOwner->GetClipNear(), _cOwner->GetClipFar());
}
