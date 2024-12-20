#include "ModelComponent.h"
#include "ActorClass.h"
#include "../ModelServer/ModelServer.h"

ModelComponent::ModelComponent(ActorClass* owner)
	:Component(owner)
	// 再生時間の初期化
	,_TotalTime(0.f)
	,_PlayTime(0.0f)
	,_AttachIndex(-1)
{
	// モデルデータのロード（テクスチャも読み込まれる）
	//_Handle = MV1LoadModel("res/Debug/chinpo.mv1");
	_Handle = ModelServer::GetInstance()->Add("res/Debug/chinpo.mv1");
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
	/*
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
	*/
	
}

void ModelComponent::SetModelInfo()
{
	// 再生時間をセットする
	MV1SetAttachAnimTime(_Handle, _AttachIndex, _PlayTime);
	// 位置
	MV1SetPosition(_Handle, VAdd(_Owner->GetPosition(),VGet(0,_Owner->GetSize().y * 100, 0)));
	// 向きからY軸回転を算出
	VECTOR vRot = { 0,0,0 };
	vRot.y = atan2(_Owner->GetDirection().x * -1, _Owner->GetDirection().z * -1);		// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
	//MV1SetRotationXYZ(_Handle, vRot);
	MV1SetScale(_Handle, _Owner->GetSize());
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
	

	// 速度を取得
	VECTOR v =_Owner -> GetMove();
	// 角度を取得
	VECTOR rot = _Owner->GetRotation();
	VECTOR rot2 = _Owner->GetRotation2();

	//	速度から角度を算出
	rot = VTransform(rot, MGetRotX(v.z/100 / _Owner->GetSize().x / 2));
	rot = VTransform(rot, MGetRotZ(-v.x/100 / _Owner->GetSize().x / 2));
	rot2 = VTransform(rot2, MGetRotX(v.z / 100 / _Owner->GetSize().x / 2));
	rot2 = VTransform(rot2, MGetRotZ(-v.x / 100 / _Owner->GetSize().x / 2));

	//	角度をセット

	MV1SetRotationZYAxis(_Model->GetHandle(),rot,rot2,0);

	_Owner->SetRotation(rot);
	_Owner->SetRotation2(rot2);

	// モデルを描画する モデルコンポでよくないか…？
	{
		_Model->SetModelInfo();
		// 描画
		MV1DrawModel(_Model->GetHandle());
		DrawFormatString(0,0,GetColor(255,255,255),"x %f, y %f, z %f",rot.x,rot.y,rot.z);
	}
}

void ModelSpriteComponent::SetImage()
{
}

