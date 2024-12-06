#pragma once
#include "appframe.h"
class CameraActor : public ActorClass
{
public:
	CameraActor(class ModeBase* mode);
	virtual ~CameraActor();

	void UpdateActor() override;

	 float GetClipNear() const { return _clipNear; }
	 void SetClipNear(float clipNear) { _clipNear = clipNear; }
	 float GetClipFar() const { return _clipFar; }
	 void SetClipFar(float clipFar) { _clipFar = clipFar; }

private:
//	VECTOR	_vTarget;				// ‹——£
	float	_clipNear, _clipFar;	// ƒNƒŠƒbƒv
};

class CameraComponent : public Component
{
public:
	CameraComponent(class CameraActor* owner,int UpdateOrder = 100);
	virtual ~CameraComponent();

	void ProcessInput() override;
	void Update() override;

private:
	class CameraActor* _cOwner;
};