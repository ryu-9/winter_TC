// HitCollisionComponent.cpp
#include "HitCollisionComponent.h"
#include "ActorClass.h"
#include "ModelComponent.h"
#include "../AppFrame/source/ModelServer/ModelServer.h"

HitCollisionComponent::HitCollisionComponent(class ActorClass* owner, ModelComponent* model, VECTOR pos, VECTOR size, int type, bool move, bool active, int handle, bool useownersize)
    : Component(owner)
    , _Model(model), Pos(pos), Size(size), Type(type), isMove(move), isActive(active), Handle(handle)
    , Rot(VGet(0, 0, 0)), Front(VGet(0, 0, 1)), Up(VGet(0, 1, 0)), OldMove(VGet(0, 0, 0)), devpos(VGet(0, 0, 0))
    , flag(false), shomen(false), _UseOwnerSize(useownersize)
{
    // モードにこのコンポーネントを追加
    _Owner->GetMode()->AddHCollision(this);

    OldPos = GetPosition();
    devpos = OldPos;
    // タイプが2以下の場合は初期化を終了
    if (type <= 2) { return; }

    // モデルコンポーネントを取得し、ハンドルを設定
    ModelComponent* modelComp = model;
    if (modelComp == nullptr) {
        auto mc = _Owner->GetComponent<ModelComponent>();
        if (mc.size() > 0) {
            modelComp = mc[0];
        }
    }

    if (Handle == -1 && modelComp != nullptr) {
        Handle = modelComp->GetHandle();
    }

    MV1SetPosition(Handle, GetPosition());
    MV1SetScale(Handle, GetSize());
    MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
    MV1SetupCollInfo(Handle);
}

HitCollisionComponent::~HitCollisionComponent()
{
    // モードからこのコンポーネントを削除
    _Owner->GetMode()->RemoveHCollision(this);
}

// 衝突判定を行い、衝突しているコンポーネントのリストを返す関数
std::deque<HitCollisionComponent*>& HitCollisionComponent::IsHit()
{
    MV1SetPosition(Handle, GetPosition());
    MV1SetScale(Handle, GetSize());
    MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
    MV1RefreshCollInfo(Handle);

    _IsHitList.clear();

    if (!isActive) { return _IsHitList; }
    for (auto hcoll : _Owner->GetMode()->GetHCollision())
    {
        if (hcoll->GetIsActive() == TRUE)
        {
            if (hcoll->GetOwner() != _Owner)
            {
                if (Type <= 2) {
                    if (hcoll->GetType() <= 2) {
                        if (HitCheck_Capsule_Capsule(GetPosition(), OldPos, GetSize().x, hcoll->GetPosition(), hcoll->GetOldPosition(), hcoll->GetSize().x)) {
                            _IsHitList.insert(_IsHitList.begin(), hcoll);
                        }
                    }
                    else {
                        auto m = MV1CollCheck_Capsule(hcoll->GetHandle(), -1, GetPosition(), GetOldPosition(), GetSize().x);
                        if (m.HitNum > 0) {
                            _IsHitList.insert(_IsHitList.begin(), hcoll);
                        }
                        MV1CollResultPolyDimTerminate(m);
                    }

                }
                else {
                    auto m = MV1CollCheck_Capsule(Handle, -1, hcoll->GetPosition(), hcoll->GetOldPosition(), hcoll->GetSize().x);
                    if (m.HitNum > 0) {
                        _IsHitList.insert(_IsHitList.begin(), hcoll);
                    }
                    MV1CollResultPolyDimTerminate(m);
                }

            }
        }
    }

    return _IsHitList;
}

// 位置を取得する関数
VECTOR HitCollisionComponent::GetPosition() {
    if (_UseOwnerSize) {
        return VAdd(_Owner->GetPosition(), VMulti(Pos, _Owner->GetSize()));
    } else {
        return VAdd(_Owner->GetPosition(), Pos);
    }
}

// サイズを取得する関数
VECTOR HitCollisionComponent::GetSize() {
    if (_UseOwnerSize) {
        return VMulti(_Owner->GetSize(), Size);
    } else {
        return Size;
    }
}

// 上方向ベクトルを取得する関数
VECTOR HitCollisionComponent::GetUp()
{
    if (Type <= 2 || _Model == nullptr) {
        return VGet(0, 1, 0);
    }
    return _Model->GetUp();
}

// 前方向ベクトルを取得する関数
VECTOR HitCollisionComponent::GetFront()
{
    if (Type <= 2 || _Model == nullptr) {
        return VGet(0, 0, 1);
    }
    return _Model->GetFront();
}

// 右方向ベクトルを取得する関数
VECTOR HitCollisionComponent::GetRight()
{
    return VCross(GetUp(), GetFront());
}

// デバッグ描画を行う関数
void HitCollisionComponent::DebugDraw()
{
    if (Type <= 2) {
        DrawCapsule3D(OldPos, GetPosition(), GetSize().x, 5, GetColor(0, 0, 255), 0, false);
    }
    else {
        VECTOR pos = GetPosition();
        MV1SetPosition(Handle, GetPosition());
        VECTOR size = GetSize();
        MV1SetScale(Handle, GetSize());
        VECTOR front = GetFront();
        VECTOR up = GetUp();
        MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
        MV1DrawModel(Handle);
    }
}

// 回転を設定する関数
void HitCollisionComponent::SetRotation(VECTOR rot)
{
    // 実装内容に応じて適切な処理を記述
}

// コリジョン情報をリフレッシュする関数
void HitCollisionComponent::RefleshCollInfo()
{
    VECTOR pos = GetPosition();
    MV1SetPosition(Handle, GetPosition());
    VECTOR size = GetSize();
    MV1SetScale(Handle, GetSize());
    MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
    MV1SetupCollInfo(Handle);
}

// 更新処理を行う関数
void HitCollisionComponent::Update()
{
    // 前回の位置を保存
    OldPos = GetPosition();
    // コリジョン情報をリフレッシュ
    RefleshCollInfo();
}
