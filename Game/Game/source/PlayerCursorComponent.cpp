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

	x = (float)input.X / 100;
	y = (float)input.Y / 100;

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
	EffectController::GetInstance()->GetShadowMap(0)->AddRemoveSprite(this);
}

PlayerCursorSpriteComponent::~PlayerCursorSpriteComponent()
{
}

void PlayerCursorSpriteComponent::Draw()
{
	if (!_ActiveFlag) { return; }
	DrawGraph(_Position.x - 258 /2, _Position.y - 237 /2, _Handle, TRUE);
}
