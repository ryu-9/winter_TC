#pragma once
#include "SpriteComponent.h"

class EffectSpriteComponent : public SpriteComponent
{
public:
	EffectSpriteComponent(ActorClass* owner, const TCHAR* file, VECTOR pos, VECTOR rot, float size = 1.0f, float playtime = -1 , float speed = 1.0f, int draworder = 100);
	~EffectSpriteComponent();
	void Draw() override;
	void SetPosition(VECTOR pos) { _Position = pos; }
	void SetRotation(VECTOR rot) { _Rotation = rot; }

	void Play();

	VECTOR debugpos[2];

private:
	int _Handle;
	int _SourceHandle;
	VECTOR _Position;
	VECTOR _Rotation;
	float _Count;
	float _PlayTime;
	VECTOR _Scale;
};

