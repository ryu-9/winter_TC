#include "CameraActor.h"

CameraActor::CameraActor(ModeBase* mode)
	:ActorClass(mode)
{
	// カメラの設定（わかりやすい位置に）
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

	// アナログスティック対応
	DINPUT_JOYSTATE di;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &di);
	float lx, ly, rx, ry;	// 左右アナログスティックの座標
	float analogMin = 0.3f;	// アナログ閾値
	// Logicoolパッドの場合
	lx = (float)di.X / 1000.f; ly = (float)di.Y / 1000.f;
	rx = (float)di.Z / 1000.f; ry = -(float)di.Rz / 1000.f;	// Y軸反転


	// Y軸回転
	float sx = _cOwner->GetPosition().x - _cOwner->GetDirection().x;
	float sz = _cOwner->GetPosition().z - _cOwner->GetDirection().z;
	float rad = atan2(sz, sx);
	float length = sqrt(sz * sz + sx * sx);
	if (rx > analogMin) { rad -= 0.05f; }
	if (rx < -analogMin) { rad += 0.05f; }
	_cOwner->SetPosition(VGet(_cOwner->GetDirection().x + cos(rad) * length,_cOwner->GetPosition().y, _cOwner->GetDirection().z + sin(rad) * length));
	// Y位置
	if (ry > analogMin) { _cOwner->SetPosition(VGet(_cOwner->GetPosition().x, _cOwner->GetPosition().y - 20.f , _cOwner->GetPosition().z));}
	if (ry < -analogMin) { _cOwner->SetPosition(VGet(_cOwner->GetPosition().x, _cOwner->GetPosition().y + 20.f, _cOwner->GetPosition().z)); }
}

void CameraComponent::Update()
{
	// カメラ設定更新
	SetCameraPositionAndTarget_UpVecY(_cOwner->GetPosition(), _cOwner->GetDirection());
	SetCameraNearFar(_cOwner->GetClipNear(), _cOwner->GetClipFar());
}
