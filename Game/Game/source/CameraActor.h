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
	 void SetFollow(class ActorClass* ac) { _Follow = ac; }
	 class ActorClass* GetFollow() { return _Follow; }
	 void SetTarget(VECTOR target) { _Taeget = target; }
	 VECTOR GetTarget() { return _Taeget; }

private:
//	VECTOR	_vTarget;				// 距離
	float	_clipNear, _clipFar;	// クリップ
	class ActorClass* _Follow;		// 追従するアクター
	class ModelComponent* _SkySphere;		// スカイスフィア
	VECTOR _Taeget;
};

class CameraComponent : public Component
{
public:
	CameraComponent(class CameraActor* owner, int UpdateOrder = 100);
	virtual ~CameraComponent();

	void ProcessInput() override;
	void Update() override;

	void SetPlayer(class PlayerActor* player1, class PlayerActor* player2);

	float GetDist() const { return _Dist; }

private:
	class CameraActor* _cOwner;
	class PlayerActor* _Player[2];
	class ShadowMapSpriteComponent* _ShadowMap[3];
	float _Dist;
	
};