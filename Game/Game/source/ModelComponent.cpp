#include "ModelComponent.h"

ModelComponent::ModelComponent(PlayerActor* owner)
	:Component(owner)
	// 再生時間の初期化
	,_pOwner(owner)
	,_TotalTime(0.f)
	,_PlayTime(0.0f)
	,_AttachIndex(-1)
{
	// モデルデータのロード（テクスチャも読み込まれる）
	_Handle = MV1LoadModel("res/SDChar/SDChar.mv1");
}

ModelComponent::~ModelComponent()
{
}

void ModelComponent::ProcessInput()
{

}

void ModelComponent::Update()
{
	// moveの分移動
//	_pOwner->SetPosition(VAdd(_pOwner->GetPosition(), _pOwner->GetMove()));

	// 移動量をそのままキャラの向きにする
	if (VSize(_pOwner->GetMove()) > 0.f) {		// 移動していない時は無視するため
		_pOwner->SetDirection(_pOwner->GetMove());
		_pOwner->SetStatus(PlayerActor::STATUS::WALK);
	}
	else {
		_pOwner->SetStatus(PlayerActor::STATUS::WAIT);
	}

	// ステータスが変わっていないか？
	if (_pOwner->IsChangeStatus()) {
		// アニメーションがアタッチされていたら、デタッチする
		if (_AttachIndex != -1) {
			MV1DetachAnim(_Handle, _AttachIndex);
			_AttachIndex = -1;
		}
		// ステータスに合わせてアニメーションのアタッチ
		switch (_pOwner->GetStatus()) {
		case PlayerActor::STATUS::WAIT:
			_AttachIndex = MV1AttachAnim(_Handle, MV1GetAnimIndex(_Handle, "idle"), -1, FALSE);
			break;
		case PlayerActor::STATUS::WALK:
			_AttachIndex = MV1AttachAnim(_Handle, MV1GetAnimIndex(_Handle, "run"), -1, FALSE);
			break;
		}
		// アタッチしたアニメーションの総再生時間を取得する
		_TotalTime = MV1GetAttachAnimTotalTime(_Handle, _AttachIndex);
		// 再生時間を初期化
		_PlayTime = 0.0f;
	}
	else {
		// 再生時間を進める
		_PlayTime += 0.5f;
		if (VSize(_pOwner->GetMove()) > 6.f) { _PlayTime += 0.3f; }
	}

	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	if (_PlayTime >= _TotalTime) {
		_PlayTime = 0.0f;
	}

	
}

void ModelComponent::SetModelInfo()
{
	// 再生時間をセットする
	MV1SetAttachAnimTime(_Handle, _AttachIndex, _PlayTime);
	// 位置
	MV1SetPosition(_Handle, _pOwner->GetPosition());
	// 向きからY軸回転を算出
	VECTOR vRot = { 0,0,0 };
	vRot.y = atan2(_pOwner->GetDirection().x * -1, _pOwner->GetDirection().z * -1);		// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
	MV1SetRotationXYZ(_Handle, vRot);
}


ModelSpriteComponent::ModelSpriteComponent(ActorClass* owner,ModelComponent* model, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	,_Model(model)
{
}

ModelSpriteComponent::~ModelSpriteComponent()
{
}

void ModelSpriteComponent::Draw()
{
	

	// モデルを描画する モデルコンポでよくないか…？
	{
		_Model->SetModelInfo();
		// 描画
		MV1DrawModel(_Model->GetHandle());
	}
}

void ModelSpriteComponent::SetImage()
{
}

