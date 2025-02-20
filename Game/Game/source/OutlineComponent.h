#pragma once
#include "appframe.h"

class OutlineComponent : public SpriteComponent
{
public:
	OutlineComponent(ActorClass* owner, const TCHAR* file, class ModelComponent* model);
	virtual ~OutlineComponent();

	void Draw() override;

private:
	int _Handle;
	int _TargetHandle;
	class CameraComponent* _Camera;
};

