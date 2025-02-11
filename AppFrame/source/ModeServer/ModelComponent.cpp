#include "ModelComponent.h"
#include "ActorClass.h"
#include "../ModelServer/ModelServer.h"

ModelComponent::ModelComponent(ActorClass* owner, const TCHAR* file)
	:Component(owner)
	// 再生時間の初期化
	,_TotalTime(0.f)
	,_PlayTime(0.0f)
	,_AttachIndex(-1)
	, _Front(VGet(0, 0, 1))
	, _Up(VGet(0, 1, 0))
	, _Rot(VGet(0, 0, 0))
	, _Position(VGet(0, 0, 0))
	, _Scale(VGet(1, 1, 1))
{
	// モデルデータのロード（テクスチャも読み込まれる）
	//_Handle = MV1LoadModel("res/Debug/chinpo.mv1");
	_Handle = ModelServer::GetInstance()->Add(file);
	new ModelSpriteComponent(_Owner, this);
}

ModelComponent::~ModelComponent()
{
}

void ModelComponent::ProcessInput()
{

}

void ModelComponent::Update()
{
	SetModelInfo();
}

void ModelComponent::SetModelInfo()
{
	// 再生時間をセットする
	MV1SetAttachAnimTime(_Handle, _AttachIndex, _PlayTime);
	// 位置
	MV1SetPosition(_Handle,VAdd(_Owner->GetPosition(), VMulti(_Position,_Owner->GetSize())));
	// 向きからY軸回転を算出
	VECTOR vRot = { 0,0,0 };
	vRot.y = atan2(_Owner->GetDirection().x * -1, _Owner->GetDirection().z * -1);		// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
	//MV1SetRotationXYZ(_Handle, vRot);
	MV1SetScale(_Handle, VMulti(_Owner->GetSize(), _Scale));
}

void ModelComponent::SetRotation(VECTOR rot)
{
	_Rot = rot;
	MV1SetRotationXYZ(_Handle, _Rot);
	VECTOR front = VGet(0, 0, 1), up = VGet(0, 1, 0);
	front = VTransform(front, MGetRotX(rot.x));
	front = VTransform(front, MGetRotY(rot.y));
	front = VTransform(front, MGetRotZ(rot.z));
	_Front = front;
	up = VTransform(up, MGetRotX(rot.x));
	up = VTransform(up, MGetRotY(rot.y));
	up = VTransform(up, MGetRotZ(rot.z));
	_Up = up;
	auto mcoll = _Owner->GetComponent<MoveCollisionComponent>();
	if (mcoll != nullptr) {
		mcoll->SetRotation(_Rot);
	}
}

void ModelComponent::SetRotationZY(VECTOR front, VECTOR up)
{
	_Front = front;
	_Up = up;
	MV1SetRotationZYAxis(_Handle, front, up, 0);
	_Rot = MV1GetRotationXYZ(_Handle);
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
	
	/*
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
	//*/

	// デバッグ描画
	_Owner->GetComponent<MoveCollisionComponent>()->DebugDraw();
	
	{
		_Model->SetModelInfo();
		// 描画
		MV1DrawModel(_Model->GetHandle());
		//DrawFormatString(0,0,GetColor(255,255,255),"x %f, y %f, z %f",rot.x,rot.y,rot.z);
	}
}

void ModelSpriteComponent::SetImage()
{
}

