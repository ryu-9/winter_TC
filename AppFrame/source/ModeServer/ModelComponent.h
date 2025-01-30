#pragma once
#include "Component.h"
#include "Dxlib.h"
#include "ModeBase.h"


class ModelComponent : public Component
{
public:
	ModelComponent(class ActorClass* owner, const TCHAR* file);
	virtual ~ModelComponent();

	void ProcessInput() override;
	void Update() override;
	void SetModelInfo();

	int GetHandle() const { return _Handle; }

	VECTOR GetFront() const { return _Front; }
	void SetFront(VECTOR front) { _Front = front; }

	VECTOR GetUp() const { return _Up; }
	void SetUp(VECTOR up) { _Up = up; }
	
	void SetVisible(bool visible) { MV1SetVisible(_Handle, visible); }

	void SetPosition(VECTOR pos) { _Position = pos; }
	VECTOR GetPosition() const { return _Position; }

	void SetScale(VECTOR scale) { _Scale = scale; }
	VECTOR GetScale() const { return _Scale; }

	void SetRotation(VECTOR rot);
	VECTOR GetRotation() const { return _Rot; }

	void SetRotationZY(VECTOR front, VECTOR up);


private:

	// 3Dƒ‚ƒfƒ‹•`‰æ—p
	int _Handle;
	float _TotalTime;
	float _PlayTime;
	int _AttachIndex;

	VECTOR _Rot;
	VECTOR _Front;
	VECTOR _Up;

	VECTOR _Position;
	VECTOR _Scale;
};

class ModelSpriteComponent : public SpriteComponent
{
public:
	ModelSpriteComponent(class ActorClass* owner,class ModelComponent* model, int drawOrder = 100);
	virtual ~ModelSpriteComponent();

	void Draw() override;
	void SetImage() override;

	
private:

	class ModelComponent* _Model;


	
};

