#include "OutlineComponent.h"
#include "CameraActor.h"
#include "ModeGame.h"
#include "ModeTestStage.h"

OutlineComponent::OutlineComponent(ActorClass* owner, const TCHAR* file, class ModelComponent* model)
	:SpriteComponent(owner)
{
	_Handle = ModelServer::GetInstance()->Add(file);
	_TargetHandle = model->GetHandle();
	auto mode = dynamic_cast<ModeGame*>(owner->GetMode());
	if (mode == nullptr) { 
		auto modeT = dynamic_cast<ModeTestStage*>(owner->GetMode());
		_Camera =modeT->GetCamera()->GetComponent<CameraComponent>()[0];

	}
	else {
		_Camera = dynamic_cast<ModeGame*>(owner->GetMode())->GetCamera()->GetComponent<CameraComponent>()[0];

	}
	auto ef = EffectController::GetInstance();
	if (ef != nullptr) {
		auto sm = ef->GetEffect<ShadowMapSpriteComponent>();
		if (sm.size() > 0) {
			sm[0]->AddRemoveSprite(this);
		}
	}
}

OutlineComponent::~OutlineComponent()
{
}

void OutlineComponent::Draw()
{
	if (MV1GetVisible(_TargetHandle)) {
		MV1SetVisible(_Handle, TRUE);
	}
	else {
		MV1SetVisible(_Handle, FALSE);
	}	
	MV1SetPosition(_Handle, MV1GetPosition(_TargetHandle));
	VECTOR scale = MV1GetScale(_TargetHandle);
	scale = VAdd(scale, VScale(VGet(1, 1, 1), _Camera->GetDist() / 10000));
	MV1SetScale(_Handle, scale);
	MV1SetRotationXYZ(_Handle, MV1GetRotationXYZ(_TargetHandle));
	//MV1DrawModel(_Handle);
}
