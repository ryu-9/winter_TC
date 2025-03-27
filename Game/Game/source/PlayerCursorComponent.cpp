#include "PlayerCursorComponent.h"

PlayerCursorComponent::PlayerCursorComponent(ActorClass* owner, int playerno)
	:Component(owner)
	, _PlayerNo(playerno)
	, _ActiveFlag(false)
{
	_Sprite = new PlayerCursorSpriteComponent(owner, 300);
}

PlayerCursorComponent::~PlayerCursorComponent()
{
}

void PlayerCursorComponent::Update()
{
	if (!_ActiveFlag) { return; }
	float x, y;
	DINPUT_JOYSTATE input;
	GetJoypadDirectInputState(_PlayerNo, &input);

	x = (float)input.X / 100 * 1.5;
	y = (float)input.Y / 100 * 1.5;

	_Position = VAdd(_Position, VGet(x, y, 0));
	if (_Position.x < 0) { _Position.x = 0; }
	if (_Position.x > 1920) { _Position.x = 1920; }
	if (_Position.y < 0) { _Position.y = 0; }
	if (_Position.y > 1080) { _Position.y = 1080; }
	_Sprite->SetPosition(_Position);

}

void PlayerCursorComponent::Init()
{
	_ActiveFlag = true;
	_Sprite->SetActiveFlag(true);
	_Position = VGet(1920 / 2, 1080 / 2, 0);
}

VECTOR PlayerCursorComponent::GetTargetDir()
{
	VECTOR v1 = ConvScreenPosToWorldPos(_Position);
	VECTOR v2 = ConvScreenPosToWorldPos(VAdd(_Position, VGet(0, 0, 1)));
	VECTOR dir = VSub(v2, v1);
	return dir;
}

VECTOR PlayerCursorComponent::GetHitPos()
{
	float dist = 1000000;
	VECTOR campos = GetCameraPosition();
	VECTOR pos = VAdd(campos, VScale(GetTargetDir(), dist));
	bool flag = false;
	for (auto mc : _Owner->GetMode()->GetMCollision())
	{
		if (mc->GetIsActive() && !mc->GetIsMove()) {
			auto result = MV1CollCheck_Line(mc->GetHandle(), -1, campos, pos, -1);
			if (result.HitFlag) {
				float tmp = VSize(VSub(campos, result.HitPosition));
				if (dist > tmp) {
					dist = tmp;
					pos = result.HitPosition;
					flag = true;

				}
			}

		}
	}
	_Sprite->debugpos[0] = campos;
	_Sprite->debugpos[1] = pos;
	return pos;
}

void PlayerCursorComponent::SetActiveFalse()
{
	_ActiveFlag = false;
	_Sprite->SetActiveFlag(false);
}

PlayerCursorSpriteComponent::PlayerCursorSpriteComponent(ActorClass* owner, int draworder)
	:SpriteComponent(owner, draworder)
	, _Position(VGet(0, 0, 0))
	, _ActiveFlag(false)
{
	_Handle = LoadGraph("res/UI/UI_AIM.png");
	auto sm = EffectController::GetInstance()->GetEffect<ShadowMapSpriteComponent>();
	if (sm.size() > 0) {
		sm[0]->AddRemoveSprite(this);
	}
}

PlayerCursorSpriteComponent::~PlayerCursorSpriteComponent()
{
}

void PlayerCursorSpriteComponent::Draw()
{
	if (!_ActiveFlag) { return; }
	DrawGraph(_Position.x - 326 / 2, _Position.y - 326 / 2, _Handle, TRUE);
	/*
		
	DrawCapsule3D(_Owner->GetPosition(), debugpos[1], 15, 5, GetColor(255, 0, 0), 0, TRUE);
	DrawCapsule3D(_Owner->GetPosition(), VAdd(_Owner->GetPosition(), VGet(0, -1000, 0)), 15, 5, GetColor(255, 0, 0), 0,TRUE);
	DrawSphere3D(debugpos[1], 40, 5, GetColor(255, 0, 0), 0, true);
	DrawLine3D(_Owner->GetPosition(), debugpos[1], GetColor(255, 0, 0));
	*/

}
