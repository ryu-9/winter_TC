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
	

private:

	// 3Dƒ‚ƒfƒ‹•`‰æ—p
	int _Handle;
	float _TotalTime;
	float _PlayTime;
	int _AttachIndex;

	VECTOR _Front;
	VECTOR _Up;
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

