#include "ShaderDemoSpriteComponent.h"
#include "OutlineComponent.h"

#include "StageBox.h"
#include "PlayerActor.h"
#include "TreeActor.h"
#include "ChangeSnowBallActor.h"


ShaderDemoSpriteComponent* ShaderDemoSpriteComponent::_Instance = nullptr;


ShaderDemoSpriteComponent::ShaderDemoSpriteComponent(ActorClass* owner, int draworder)
	: SpriteComponent(owner, draworder)
	, _DrawFlag(false)
{
	_Instance = this;

	_ReflectHandle = MakeScreen(2048, 1024, TRUE); // 反射用の描画先スクリーンを作成

	//_MatCapShader[0] = LoadVertexShader("res/shader/NormalMesh_DirLight_NrmMapVS.vso");
	_MatCapShader[0] = LoadVertexShader("res/shader/MatCapVS.vso");
	//_MatCapShader[1] = LoadVertexShader("res/shader/SkinMesh4_DirLightVS.vso");
	_MatCapShader[1] = LoadVertexShader("res/shader/MatCap2VS.vso");
	_MatCapShader[2] = LoadGeometryShader("res/shader/MatCapGS.pso");

	_VShader[0] = LoadVertexShader("res/shader/NormalMesh_DirLight_NrmMapVS.vso");
	_PShader[0] = LoadPixelShader("res/shader/NormalMesh_DirLight_NrmMapPS.pso");
}

ShaderDemoSpriteComponent::~ShaderDemoSpriteComponent()
{
}

void ShaderDemoSpriteComponent::Draw()
{
	//CreateZMask();
	CreateReflect();
}

void ShaderDemoSpriteComponent::CreateZMask()
{
	if (_DrawFlag) return;
	_DrawFlag = true;
	
	_Owner->GetMode()->Render();

	_DrawFlag = false;
}

void ShaderDemoSpriteComponent::CreateReflect()
{
	if(_DrawFlag) return;
	_DrawFlag = true;


	VECTOR cTarget = GetCameraTarget();
	VECTOR cDir = VNorm(VSub(cTarget, GetCameraPosition())); // カメラの方向ベクトルを取得
	VECTOR cPos = GetCameraPosition();
	float nearZ = GetCameraNear();
	float farZ = GetCameraFar();
	float angle = GetCameraFov();
	SetDrawScreen(_ReflectHandle); // 反射グラフを描画先に設定
	ClearDrawScreen(); // 描画先をクリア
	VECTOR newcPos = VAdd(cPos, VScale(cDir, 2.0 * nearZ)); // カメラの位置を反射位置に設定
	VECTOR newcTarget = VAdd(newcPos, VScale(cDir, -VSize(VSub(cPos, cTarget)))); // カメラのターゲットを反射位置に設定
	SetCameraPositionAndTargetAndUpVec(newcPos, cPos, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定

	DrawBox(0, 0, 150, 150, GetColor(255, 255, 255), TRUE);
	
	MATRIX P = {};
	float aspect = 2;
	float fovY = 0;
	float C = 1.0f / tanf(fovY * 0.5f);
	float A = C / aspect;
	float zn = 1e-4f;     // 小さな正
	float zf = 1e6f;      // 大きい
	float D = zf / (zf - zn);
	float E = -zn * zf / (zf - zn);
	float Sx = 0;
	float Sy = 0;

	P.m[0][0] = A;    P.m[0][2] = Sx;
	P.m[1][1] = C;    P.m[1][2] = Sy;
	P.m[2][2] = D;    P.m[2][3] = 1.0f;
	P.m[3][2] = E;    P.m[3][3] = 0.0f;

	MV1SetUseOrigShader(TRUE);

	//SetupCamera_ProjectionMatrix(P);

	//SetUseVertexShader(_VShader[1]); // 反射用の頂点シェーダーを設定

	SetUsePixelShader(_PShader[0]); // 反射用のピクセルシェーダーを設定

	//SetUseGeometryShader(_PShader[0]);


	SetVSConstF(20, { nearZ, farZ, 0, 0 });
	
	//SetUseBackCulling(DX_CULLING_RIGHT);

	for(auto s: _Owner->GetMode()->GetSprites()) {
		if (dynamic_cast<OutlineComponent*>(s) != nullptr) continue;
		auto null = dynamic_cast<EffectManager*>(s);
		if (null == nullptr) {
			if (s->GetDrawOrder() < 0 || s -> GetDrawOrder() > 150) continue; // 描画順序が負のスプライトは描画しない
			auto m = dynamic_cast<ModelSpriteComponent*>(s);
			if( m != nullptr) {
				if (dynamic_cast<ChangeSnowBallActor*>(s->GetOwner()) != nullptr) {
					int test = 0;
				}
				int handle = m->GetModel()->GetHandle();
				int type = MV1GetTriangleListVertexType(handle, 0);
				if (type == DX_MV1_VERTEX_TYPE_1FRAME || type == DX_MV1_VERTEX_TYPE_NMAP_1FRAME) {
					SetUseVertexShader(_MatCapShader[0]); // 法線マップ用の頂点シェーダーを設定
					if (dynamic_cast<TreeActor*>(s->GetOwner()) != nullptr) {
						SetUseVertexShader(_MatCapShader[1]); // マテリアルキャップ用の頂点シェーダーを設定
					}
				}
				else {

					SetUseVertexShader(_MatCapShader[1]); // マテリアルキャップ用の頂点シェーダーを設定
				}

			}
			else {
				SetUseVertexShader(_MatCapShader[0]);
			}

			s->Draw();
		}
	}

	MV1SetUseOrigShader(FALSE);
	//SetUseVertexShader(-1); // 頂点シェーダーを無効化
	//SetUseGeometryShader(-1); // ジオメトリシェーダーを無効化
	//SetUseBackCulling(DX_CULLING_RIGHT);

	VECTOR dbugpos = GetCameraPosition();
	VECTOR dbugtarget = GetCameraTarget();

	SetDrawScreen(DX_SCREEN_BACK); // 描画先をバックスクリーンに戻す
	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを元に戻す
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定

	_DrawFlag = false;
}
