#pragma once

#include "appframe.h"
#include "DxLib.h"

class PlayerCursorComponent : public Component
{
public:
	PlayerCursorComponent(ActorClass* owner, int playerno);
	~PlayerCursorComponent();
	void Update() override;

	void Init();

	VECTOR GetTargetDir();

	void SetActiveFalse();

private:
	int _PlayerNo;
	bool _ActiveFlag;
	VECTOR _Position;
	class PlayerCursorSpriteComponent* _Sprite;
};


class PlayerCursorSpriteComponent : public SpriteComponent
{
public:
	PlayerCursorSpriteComponent(ActorClass* owner, int draworder);
	~PlayerCursorSpriteComponent();
	void Draw() override;

	void SetPosition(VECTOR pos) { _Position = pos; }

	void SetActiveFlag(bool flag) { _ActiveFlag = flag; }

private:
	int _Handle;
	VECTOR _Position;
	bool _ActiveFlag;
};