#include "ShaderDemoSpriteComponent.h"
#include "OutlineComponent.h"

#include "StageBox.h"
#include "PlayerActor.h"
#include "TreeActor.h"
#include "ChangeSnowBallActor.h"
#include "SnowComponent.h"

#include "CameraActor.h"
#include "ModeGame.h"

ShaderDemoSpriteComponent* ShaderDemoSpriteComponent::_Instance = nullptr;


ShaderDemoSpriteComponent::ShaderDemoSpriteComponent(ActorClass* owner, int draworder)
	: SpriteComponent(owner, draworder)
	, _DrawFlag(false)
{
	_Instance = this;
	

	_ReflectHandle = MakeScreen(2048, 1024, TRUE); // 反射用の描画先スクリーンを作成
	SetUseGraphZBuffer(_ReflectHandle, TRUE, 16);

	//_MatCapShader[0] = LoadVertexShader("res/shader/NormalMesh_DirLight_NrmMapVS.vso");
	_MatCapShader[0] = LoadVertexShader("res/shader/MatCapVS.vso");
	//_MatCapShader[1] = LoadVertexShader("res/shader/SkinMesh4_DirLightVS.vso");
	_MatCapShader[1] = LoadVertexShader("res/shader/MatCap2VS.vso");
	_MatCapShader[2] = LoadGeometryShader("res/shader/MatCapGS.pso");

	_MatCapShader[3] = LoadPixelShader("res/shader/MatCapPS.pso");

	_VShader[0] = LoadVertexShader("res/shader/NormalMesh_DirLight_NrmMapVS.vso");
	_VShader[1] = LoadVertexShader("res/shader/SkinMesh4_DirLightVS.vso");
	_PShader[0] = LoadPixelShader("res/shader/NormalMesh_DirLight_NrmMapPS.pso");
	_PShader[1] = LoadPixelShader("res/shader/ZBuffer1_PS.pso");
	_PShader[2] = LoadPixelShader("res/shader/ZBuffer2_PS.pso");
	_PShader[3] = LoadPixelShader("res/shader/ZBuffer3_PS.pso");
	_PShader[4] = LoadPixelShader("res/shader/merikomiPS.pso");

	_ZMaskHandle[0] = MakeScreen(1920, 1080, TRUE); // Zマスク用の描画先スクリーンを作成
	_ZMaskHandle[1] = MakeScreen(1920, 1080, TRUE); // Zマスク用の描画先スクリーンを作成
	_ZMaskHandle[2] = MakeScreen(1920, 1080, TRUE); // Zマスク用の描画先スクリーンを作成
}

ShaderDemoSpriteComponent::~ShaderDemoSpriteComponent()
{
}

void ShaderDemoSpriteComponent::Draw()
{
	CreateZMask();
	CreateReflect();
}

void ShaderDemoSpriteComponent::CreateZMask()
{
	if (_DrawFlag) return;
	_DrawFlag = true;
	
	VECTOR cTarget = GetCameraTarget();
	VECTOR cPos = GetCameraPosition();
	float nearZ = GetCameraNear();
	float farZ = GetCameraFar();
	SetWriteZBufferFlag(FALSE);
	for (int i = 0; i < 3; i++) {
		SetDrawScreen(_ZMaskHandle[i]); // Zバッファを使用するための描画先を設定
		ClearDrawScreen(); // 描画先をクリア
		if (i != 0)
			DrawBox(0, 0, 1920, 1080, GetColor(255, 255, 255), TRUE); // バックスクリーンをZバッファに描画
		else
			DrawBox(0, 0, 1920, 1080, GetColor(255, 255, 255), TRUE); // バックスクリーンをZバッファに描画
	}
	DrawBox(0, 0, 1920, 1080, GetColor(255, 255, 255), TRUE); // バックスクリーンをZバッファに描画

	SetDrawScreen(_ZMaskHandle[2]); // Zバッファを使用するための描画先を設定
	SetWriteZBufferFlag(TRUE);

	MV1SetUseOrigShader(TRUE);
	SetUseVertexShader(_VShader[0]);
	SetUsePixelShader(_PShader[0]);
	//SetRenderTargetToShader(0, _ZMaskHandle[2]);

	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定

	
	for (auto s : _Owner->GetMode()->GetSprites()) {
		int VshaderHandle = -1;
		int PshaderHandle = -1;
		bool isShader = false;
		if (s->GetDrawOrder() < 0 || s->GetDrawOrder() > 150) continue; // 描画順序が負のスプライトは描画しない

		if (dynamic_cast<OutlineComponent*>(s) != nullptr) continue;
		if (dynamic_cast<EffectManager*>(s) != nullptr) continue;
		auto m = dynamic_cast<ModelSpriteComponent*>(s);
		if (m == nullptr) continue; // モデルスプライトでない場合は描画しない
		if (dynamic_cast<StageBox*>(s->GetOwner()) == nullptr) continue; // ステージボックス以外描画しない

		VshaderHandle = m->GetShaderHandle(0);
		PshaderHandle = m->GetShaderHandle(1);
		isShader = m->GetUseShader();
		m->SetUseShader(true); // モデルスプライトのシェーダーを無効化

		if (dynamic_cast<StageBox*>(s->GetOwner()) != nullptr) {
			int test = 0;
		}
		int handle = m->GetModel()->GetHandle();

		int type = MV1GetTriangleListVertexType(handle, 0);
		if (type == DX_MV1_VERTEX_TYPE_1FRAME || type == DX_MV1_VERTEX_TYPE_NMAP_1FRAME) {
			//SetUseVertexShader(_MatCapShader[0]); // 法線マップ用の頂点シェーダーを設定
			m->SetShaderHandle(_VShader[0]); // モデルスプライトのシェーダーを設定
			if (dynamic_cast<TreeActor*>(s->GetOwner()) != nullptr) {

				m->SetShaderHandle(_VShader[1]); // モデルスプライトのシェーダーを設定
			}
		}
		else {


			m->SetShaderHandle(_VShader[1]); // モデルスプライトのシェーダーを設定
		}
		m->SetShaderHandle(_PShader[1], 1); // Zバッファ用のピクセルシェーダーを設定


		s->Draw();
		m->SetUseShader(isShader); // モデルスプライトのシェーダーを元に戻す
		m->SetShaderHandle(VshaderHandle, 0); // モデルスプライトのシェーダーを元に戻す
		m->SetShaderHandle(PshaderHandle, 1); // モデルスプライトのシェーダーを元に戻す
	}
	
	//SetRenderTargetToShader(0, -1);


	SetDrawScreen(DX_SCREEN_BACK); // Zバッファを使用するための描画先を設定
	ClearDrawScreen(); // 描画先をクリア

	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定
	_DrawFlag = false;
	return;
	/*
	std::vector<ShaderDemoSubSpriteComponent*> ssc; // カメラからの距離が遠い順にソートするためのベクター
	float dist = 0.0f;
	for (auto sc : _DepressedList) {
		bool flag = false;
		dist = VSize(VSub(sc->GetOwner()->GetPosition(), cPos)); // カメラからの距離を計算
		for (int i = 0; i < ssc.size(); i++) {
			if (dist > VSize(VSub(ssc[i]->GetOwner()->GetPosition(), cPos))) {
				ssc.insert(ssc.begin() + i, sc); // カメラからの距離が遠い順に挿入
				flag = true;
				break;
			}
		}
		if (!flag) {
			ssc.push_back(sc); // どこにも挿入されなかった場合は最後に追加
		}
	}
	int i = 0;
	for (auto sc : ssc) {
		if (sc) {
			sc->DrawMask(i); // 押し込まれたスプライトのマスクを描画
			i++;
		}
	}
	*/


	SetDrawScreen(DX_SCREEN_BACK); // Zバッファを使用するための描画先を設定
	SetWriteZBufferFlag(TRUE);


	SetUseVertexShader(_VShader[0]);
	SetUsePixelShader(_PShader[0]);
	SetRenderTargetToShader(0, _ZMaskHandle[2]);
	ClearDrawScreen();

	//MV1SetMeshBackCulling(_ModelHandle, 0, DX_CULLING_RIGHT);
	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定

	//MV1DrawModel(_ModelHandle); // モデルを描画
	SetRenderTargetToShader(0, -1);
	//MV1SetMeshBackCulling(_ModelHandle, 0, DX_CULLING_LEFT);

	SetDrawScreen(DX_SCREEN_BACK); // Zバッファを使用するための描画先を設定
	ClearDrawScreen(); // 描画先をクリア
	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定



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
	//SetRenderTargetToShader(0, _ReflectHandle);
	ClearDrawScreen(); // 描画先をクリア
	VECTOR newcPos = VAdd(cPos, VScale(cDir, 2.0 * nearZ)); // カメラの位置を反射位置に設定
	VECTOR newcTarget = VAdd(newcPos, VScale(cDir, -VSize(VSub(cPos, cTarget)))); // カメラのターゲットを反射位置に設定
	
	SetCameraPositionAndTargetAndUpVec(newcPos, cPos, VGet(0, 1, 0)); // カメラの位置と向きを設定
	//SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
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

	SetupCamera_ProjectionMatrix(P);

	//SetUseVertexShader(_VShader[1]); // 反射用の頂点シェーダーを設定

	SetUsePixelShader(_PShader[0]); // 反射用のピクセルシェーダーを設定

	SetUseGeometryShader(_MatCapShader[2]);

	const int cbSize = sizeof(FLOAT4);                                              // 16B
	int cbUserDepth = CreateShaderConstantBuffer(cbSize);
	auto* p = (FLOAT4*)GetBufferShaderConstantBuffer(cbUserDepth);                  // マッピング
	p->x = nearZ;                // near
	p->y = farZ;                // far
	p->z = 0.0f; p->w = 0.0f; // パディング
	UpdateShaderConstantBuffer(cbUserDepth);                                        // GPU へ反映
	SetShaderConstantBuffer(cbUserDepth, DX_SHADERTYPE_VERTEX, 4);

	
	//SetUseBackCulling(DX_CULLING_RIGHT);

	SetUseTextureToShader(15,-1);

	for(auto s: _Owner->GetMode()->GetSprites()) {
		int shaderHandle = -1;
		bool isShader = false;

		if (dynamic_cast<OutlineComponent*>(s) != nullptr) continue;
		auto null = dynamic_cast<EffectManager*>(s);
		if (null == nullptr) {
			if (s->GetDrawOrder() < 0 || s -> GetDrawOrder() > 150) continue; // 描画順序が負のスプライトは描画しない
			auto m = dynamic_cast<ModelSpriteComponent*>(s);
			if( m != nullptr) {
				shaderHandle = m->GetShaderHandle(0);
				isShader = m->GetUseShader();
				m->SetUseShader(true); // モデルスプライトのシェーダーを無効化

				if (dynamic_cast<StageBox*>(s->GetOwner()) != nullptr) {
					int test = 0;
				}
				int handle = m->GetModel()->GetHandle();

				int type = MV1GetTriangleListVertexType(handle, 0);
				if (type == DX_MV1_VERTEX_TYPE_1FRAME || type == DX_MV1_VERTEX_TYPE_NMAP_1FRAME) {
					//SetUseVertexShader(_MatCapShader[0]); // 法線マップ用の頂点シェーダーを設定
					m->SetShaderHandle(_MatCapShader[0]); // モデルスプライトのシェーダーを設定
					if (dynamic_cast<TreeActor*>(s->GetOwner()) != nullptr) {
						//SetUseVertexShader(_MatCapShader[1]); // マテリアルキャップ用の頂点シェーダーを設定
						m->SetShaderHandle(_MatCapShader[1]); // モデルスプライトのシェーダーを設定
					}
				}
				else {

					//SetUseVertexShader(_MatCapShader[1]); // マテリアルキャップ用の頂点シェーダーを設定
					m->SetShaderHandle(_MatCapShader[1]); // モデルスプライトのシェーダーを設定
				}

			}
			else {
				SetUseVertexShader(_MatCapShader[0]);
			}

			SetShaderConstantBuffer(cbUserDepth, DX_SHADERTYPE_VERTEX, 4);
			auto ss = dynamic_cast<SnowComponent*>(s);
			if (ss != nullptr)  ss->DrawForReflect();
			else s->Draw();
			if (m != nullptr) {
				m->SetUseShader(isShader); // モデルスプライトのシェーダーを元に戻す
				m->SetShaderHandle(shaderHandle); // モデルスプライトのシェーダーを元に戻す
			}
		}
	}

	SetUseGeometryShader(-1); // ジオメトリシェーダーを無効化

	//SetUseVertexShader(_VShader[0]); // マテリアルキャップ用の頂点シェーダーを設定
	SetUsePixelShader(_MatCapShader[3]); // マテリアルキャップ用のピクセルシェーダーを設定
	SetUseTextureToShader(15, _ReflectHandle); // 反射用のテクスチャをシェーダーに設定

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



ShaderDemoSubActor::ShaderDemoSubActor(ModeBase* mode, const VECTOR pos, VECTOR scale)
	: ActorClass(mode)
{
	SetPosition(pos);
	SetSize(scale);
	auto model = new ModelComponent(this, ""); // モデルコンポーネントを追加
	new ShaderDemoSubSpriteComponent(this, model); // スプライトコンポーネントを追加
}

ShaderDemoSubSpriteComponent::ShaderDemoSubSpriteComponent(ActorClass* owner, ModelComponent* model, int drawOrder)
	: ModelSpriteComponent(owner, model)
{
	_VShader[0] = LoadVertexShader("res/merikomi/ZBuffer3_VS.vso");
	_PShader[0] = LoadPixelShader("res/merikomi/ZBuffer3_PS.pso");
	_VShader[1] = LoadVertexShader("res/merikomi/ZBuffer2_VS.vso");
	_PShader[1] = LoadPixelShader("res/merikomi/ZBuffer2_PS.pso");
	_VShader[2] = LoadVertexShader("res/merikomi/merikomiVS.vso");
	//_VShader[2] = LoadVertexShader("res/hansha/MatCapVS.vso");
	_PShader[2] = LoadPixelShader("res/merikomi/merikomiPS.pso");
	//_PShader[2] = LoadPixelShader("res/hansha/NormalMesh_DirLight_NrmMapPS.pso");
	_BackCullMask = MakeScreen(1920, 1080, TRUE); // バックカリング用の描画先を作成
	_ZBufferMask[0] = ShaderDemoSpriteComponent::GetInstance()->GetZMaskHandle(0); // Zマスクのハンドルを取得
	_ZBufferMask[1] = ShaderDemoSpriteComponent::GetInstance()->GetZMaskHandle(1); // Zマスクのハンドルを取得
	_ZBufferMask[2] = ShaderDemoSpriteComponent::GetInstance()->GetZMaskHandle(2); // Zマスクのハンドルを取得

}



void ShaderDemoSubSpriteComponent::DrawMask(int i)
{
	VECTOR cTarget = GetCameraTarget();
	VECTOR cPos = GetCameraPosition();
	float nearZ = GetCameraNear();
	float farZ = GetCameraFar();


	SetDrawScreen(_BackCullMask);
	ClearDrawScreen(); // 描画先をクリア

	int num = GetMultiDrawScreenNum();

	SetUseZBufferFlag(TRUE); // Zバッファを使用する
	//MV1SetUseOrigShader(TRUE); // オリジナルのシェーダーを使用する

	SetUseVertexShader(_VShader[1]);
	SetUsePixelShader(_PShader[1]);

	SetDrawScreen(DX_SCREEN_BACK); // 描画先をバックスクリーンに設定
	//ClearDrawScreen();
	SetRenderTargetToShader(0, _ZBufferMask[1]);
	SetRenderTargetToShader(1, _BackCullMask); // Zバッファをシェーダーに設定
	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定

	SetUseTextureToShader(2, _ZBufferMask[2]); // 法線マップをシェーダーに設定
	SetUseTextureToShader(3, _ZBufferMask[0]); // Zバッファをシェーダーに設定

	MV1SetUseZBuffer(_Model->GetHandle(), TRUE); // Zバッファを使用しない
	MV1SetWriteZBuffer(_Model->GetHandle(), TRUE); // Zバッファに書き込まない
	MV1SetMeshBackCulling(_Model->GetHandle(), 0, DX_CULLING_RIGHT); // メッシュのバックカリングを無効化

	MV1DrawModel(_Model->GetHandle()); // モデルを描画

	MV1SetUseZBuffer(_Model->GetHandle(), TRUE); // Zバッファを使用する
	MV1SetWriteZBuffer(_Model->GetHandle(), TRUE); // Zバッファに書き込む
	MV1SetMeshBackCulling(_Model->GetHandle(), 0, DX_CULLING_LEFT); // メッシュのバックカリングを有効化

	SetUseVertexShader(_VShader[0]);
	SetUsePixelShader(_PShader[0]);

	//SetDrawScreen(DX_SCREEN_BACK); // 描画先をバックスクリーンに設定

	SetDrawScreen(DX_SCREEN_BACK); // 描画先をバックスクリーンに設定
	SetRenderTargetToShader(1, -1);
	ClearDrawScreen();
	SetRenderTargetToShader(0, _ZBufferMask[0]);


	//SetDrawScreen(_ZBufferMask[0]);
	ClearDrawScreenZBuffer(); // Zバッファをクリア
	//MV1SetZBufferCmpType(DX_CMP_GREATER);

	SetWriteZBufferFlag(TRUE);
	SetUseZBufferFlag(TRUE); // Zバッファを使用する	
	MV1SetUseZBuffer(_Model->GetHandle(), TRUE); // Zバッファを使用する
	MV1SetWriteZBuffer(_Model->GetHandle(), TRUE); // Zバッファに書き込む
	MV1SetMeshBackCulling(_Model->GetHandle(), 0, DX_CULLING_LEFT); // メッシュのバックカリングを有効化

	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定
	SetUseTextureToShader(2, _ZBufferMask[1]);
	SetUseTextureToShader(3, _BackCullMask); // 法線マップをシェーダーに設定
	MV1DrawModel(_Model->GetHandle());

	//SetZBufferCmpType(DX_CMP_LESS);
	SetDrawScreen(DX_SCREEN_BACK); // 描画先をバックスクリーンに設定
	ClearDrawScreen();
	SetRenderTargetToShader(0, -1);


	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定
}