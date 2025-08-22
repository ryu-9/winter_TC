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
	_PShader[3] = LoadPixelShader("res/shader/ZBuffer4_PS.pso");
	_PShader[4] = LoadPixelShader("res/shader/merikomiPS.pso");
	_PShader[5] = LoadPixelShader("res/shader/ZBuffer5_PS.pso");

	_ZMaskHandle[0] = MakeScreen(1920, 1080, TRUE); // Zマスク用の描画先スクリーンを作成
	_ZMaskHandle[1] = MakeScreen(1920, 1080, TRUE); // Zマスク用の描画先スクリーンを作成
	_ZMaskHandle[2] = MakeScreen(1920, 1080, TRUE); // Zマスク用の描画先スクリーンを作成
	_ZMaskHandle[3] = MakeScreen(1920, 1080, TRUE); // Zマスク用の描画先スクリーンを作成
	_ZMaskHandle[4] = MakeScreen(1920, 1080, TRUE); // Zマスク用の描画先スクリーンを作成
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
	for (int i = 0; i < 4; i++) {
		SetDrawScreen(_ZMaskHandle[i]); // Zバッファを使用するための描画先を設定
		ClearDrawScreen(); // 描画先をクリア
		if (i == 2 || 1 == 0 ) {}
			//DrawBox(0, 0, 1920, 1080, GetColor(255, 255, 255), TRUE); // バックスクリーンをZバッファに描画
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

	std::vector<ShaderDemoSubSpriteComponent*> ssc; // 押し込まれたスプライトのリスト
	std::list<ModelSpriteComponent*> modelSprites; // モデルスプライトのリスト

	for (auto s : _Owner->GetMode()->GetSprites()) {
		int VshaderHandle = -1;
		int PshaderHandle = -1;
		bool isShader = false;
		if (s->GetDrawOrder() < 0 || s->GetDrawOrder() > 150) continue; // 描画順序が負のスプライトは描画しない
		auto sds = dynamic_cast<ShaderDemoSubSpriteComponent*>(s);
		if (sds != nullptr) {
			float dist = 0.0f;
			bool flag = false;
			dist = VSize(VSub(sds->GetOwner()->GetPosition(), cPos)); // カメラからの距離を計算
			for (int i = 0; i < ssc.size(); i++) {
				if (dist > VSize(VSub(ssc[i]->GetOwner()->GetPosition(), cPos))) {
					ssc.insert(ssc.begin() + i, sds); // カメラからの距離が遠い順に挿入
					flag = true;
					break;
				}
			}
			if (!flag) {
				ssc.push_back(sds); // どこにも挿入されなかった場合は最後に追加
			}
			continue; // ShaderDemoSubSpriteComponentは描画しない
		}
		if (dynamic_cast<OutlineComponent*>(s) != nullptr) continue;
		if (dynamic_cast<EffectManager*>(s) != nullptr) continue;
		auto m = dynamic_cast<ModelSpriteComponent*>(s);
		if (m == nullptr) continue; // モデルスプライトでない場合は描画しない
		if (dynamic_cast<StageBox*>(s->GetOwner()) == nullptr) continue; // ステージボックス以外描画しない

		VshaderHandle = m->GetShaderHandle(0);
		PshaderHandle = m->GetShaderHandle(1);
		isShader = m->GetUseShader();
		m->SetUseShader(true); // モデルスプライトのシェーダーを無効化

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

		modelSprites.emplace_back(m); // モデルスプライトをリストに追加
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
	//_DrawFlag = false;
	//return;

	for(auto sds : ssc) {
		sds->DrawMask();
	}

	SetWriteZBufferFlag(FALSE); // Zバッファの書き込みを無効化
	SetDrawScreen(_ZMaskHandle[2]); // Zバッファを使用するための描画先を設定
	ClearDrawScreen(); // 描画先をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(255, 255, 255), TRUE); // バックスクリーンを描画


	SetDrawScreen(_ZMaskHandle[3]); // Zバッファを使用するための描画先を設定
	SetWriteZBufferFlag(TRUE);


	SetUseVertexShader(_VShader[0]);
	SetUsePixelShader(_PShader[0]);
	//SetRenderTargetToShader(0, _ZMaskHandle[2]);
	ClearDrawScreen();


	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定
	
	
	//DrawBox(0, 0, 1920, 1080, GetColor(255, 255, 255), TRUE); // バックスクリーンを描画
	SetWriteZBufferFlag(TRUE); // Zバッファの書き込みを有効化
	for(auto m : modelSprites) {
		SetDrawScreen(_ZMaskHandle[4]); // Zバッファを使用するための描画先を設定
		ClearDrawScreen(); // 描画先をクリア
		SetDrawScreen(_ZMaskHandle[3]); // Zバッファを使用するための描画先を設定
		SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
		SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定
		SetRenderTargetToShader(1, _ZMaskHandle[4]); // Zバッファを使用するための描画先を設定

		m->SetUseCullingSetting(DX_CULLING_RIGHT); // モデルスプライトの背面カリングを無効化
		//SetUseBackCulling(DX_CULLING_NONE); // モデルスプライトの背面カリングを有効化
		int shaderHandle = m->GetShaderHandle(1);
		int VshaderHandle = m->GetShaderHandle(0);	
		int TextureHandle[2] = { m->GetTextureHandle(13), m->GetTextureHandle(14) };

		bool isShader = m->GetUseShader();
		m->SetUseShader(true); // モデルスプライトのシェーダーを無効化
		m->SetShaderHandle(_VShader[0], 0); // モデルスプライトのシェーダーを設定
		m->SetShaderHandle(_PShader[3], 1); // Zバッファ用のピクセルシェーダーを設定
		m->Draw();
		SetRenderTargetToShader(1, -1);
		
		SetDrawScreen(_ZMaskHandle[2]); // Zバッファを使用するための描画先を設定
		SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
		SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定

		SetUseTextureToShader(13, -1); // Zマスクをシェーダーから削除
		m->SetUseCullingSetting(DX_CULLING_LEFT); // モデルスプライトの背面カリングを有効化
		m->AddShaderTexture(13, -1); // モデルスプライトのテクスチャをZマスクに設定
		m->AddShaderTexture(14, _ZMaskHandle[4]); // モデルスプライトのテクスチャを元に戻す
		m->SetShaderHandle(_PShader[5], 1); // Zバッファ用のピクセルシェーダーを設定
		m->Draw(); // モデルスプライトを描画
		


		m->SetUseShader(isShader); // モデルスプライトのシェーダーを元に戻す
		m->SetShaderHandle(VshaderHandle, 0); // モデルスプライトのシェーダーを元に戻す
		m->SetShaderHandle(shaderHandle, 1); // モデルスプライトのシェーダーを元に戻す
		m->AddShaderTexture(13, TextureHandle[0]); // モデルスプライトのテクスチャを元に戻す
		m->AddShaderTexture(14, TextureHandle[1]); // モデルスプライトのテクスチャを元に戻す
		//MV1SetMeshBackCulling(m->GetModel()->GetHandle(), 1, DX_CULLING_LEFT);
		
	}


	SetUseTextureToShader(13, -1); // Zマスクをシェーダーから削除
	//MV1DrawModel(_ModelHandle); // モデルを描画
	SetRenderTargetToShader(0, -1);
	SetRenderTargetToShader(1, -1);
	//DrawBox(0, 0, 1920, 1080, GetColor(255, 255, 255), TRUE); // バックスクリーンを描画

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
	if (p == NULL) {
		SetDrawScreen(DX_SCREEN_BACK); // 描画先をバックスクリーンに戻す
		SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを元に戻す
		SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定
		return; // マッピングに失敗した場合は終了
	}
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
	, _Count(200000)
{
	SetPosition(pos);
	SetSize(scale);
	auto model = new ModelComponent(this, "res/Stage/model/Sphere.mv1"); // モデルコンポーネントを追加
	//auto model = new ModelComponent(this, "res/model/Mapchip/Mapchip_Broken1.mv1"); // モデルコンポーネントを追加
	delete model->GetSprite(); // 既存のスプライトを削除
	new ShaderDemoSubSpriteComponent(this, model); // スプライトコンポーネントを追加
}


ShaderDemoSubActor::~ShaderDemoSubActor()
{
	// デストラクタで特に何もしない
}

void ShaderDemoSubActor::UpdateActor()
{
	_Count -= 1000 / GetFPS(); // カウントダウン

	if(_Count<= 0) {
		delete this; // カウントが0以下になったら自分自身を削除
	}
}

ShaderDemoSubSpriteComponent::ShaderDemoSubSpriteComponent(ActorClass* owner, ModelComponent* model, int drawOrder)
	: ModelSpriteComponent(owner, model)
{
	_VShader[0] = LoadVertexShader("res/Shader/ZBuffer3_VS.vso");
	_PShader[0] = LoadPixelShader("res/Shader/ZBuffer3_PS.pso");
	_VShader[1] = LoadVertexShader("res/Shader/ZBuffer2_VS.vso");
	_PShader[1] = LoadPixelShader("res/Shader/ZBuffer2_PS.pso");
	//_VShader[2] = LoadVertexShader("res/Shader/merikomiVS.vso");
	_VShader[2] = LoadVertexShader("res/shader/NormalMesh_DirLight_NrmMapVS.vso");
	_PShader[2] = LoadPixelShader("res/Shader/merikomiPS.pso");
	_PShader[3] = LoadPixelShader("res/Shader/ZBuffer1_PS.pso");
	//_PShader[2] = LoadPixelShader("res/shader/NormalMesh_DirLight_NrmMapPS.pso");
	_BackCullMask = MakeScreen(1920, 1080, TRUE); // バックカリング用の描画先を作成
	_ZBufferMask[0] = ShaderDemoSpriteComponent::GetInstance()->GetZMaskHandle(0); // Zマスクのハンドルを取得
	_ZBufferMask[1] = ShaderDemoSpriteComponent::GetInstance()->GetZMaskHandle(1); // Zマスクのハンドルを取得
	_ZBufferMask[2] = ShaderDemoSpriteComponent::GetInstance()->GetZMaskHandle(2); // Zマスクのハンドルを取得
	_ZBufferMask[3] = ShaderDemoSpriteComponent::GetInstance()->GetZMaskHandle(3); // Zマスクのハンドルを取得
	_ZBufferMask[4] = ShaderDemoSpriteComponent::GetInstance()->GetZMaskHandle(4); // Zマスクのハンドルを取得
	//_ZBufferMask[3] = MakeScreen(1920, 1080, TRUE); // Zマスク用の描画先を作成
	//_ZBufferMask[4] = MakeScreen(1920, 1080, TRUE); // Zマスク用の描画先を作成
	_BackCullMask = MakeScreen(1920, 1080, TRUE); // バックカリング用の描画先を作成
	
	SetUseShader(true); // シェーダーを使用する
	SetShaderHandle(_VShader[2], 0); // 頂点シェーダーを設定
	SetShaderHandle(_PShader[2], 1); // ピクセルシェーダーを設定

}

ShaderDemoSubSpriteComponent::~ShaderDemoSubSpriteComponent()
{
	// debug
	//DeleteGraph(_ZBufferMask[3]); // Zマスク用の描画先を削除
	//DeleteGraph(_ZBufferMask[4]); // Zマスク用の描画先を削除
	DeleteGraph(_BackCullMask); // バックカリング用の描画先を削除
}



void ShaderDemoSubSpriteComponent::Draw()
{

	// モデルのテクスチャハンドルを取得
	MV1SetMeshBackCulling(_Model->GetHandle(), 0, DX_CULLING_NONE);
	MV1SetUseOrigShader(TRUE);

	SetUseTextureToShader(12, _ZBufferMask[2]); // 法線マップをシェーダーに設定
	SetUseTextureToShader(13, _ZBufferMask[1]);
	SetUseTextureToShader(14, _ZBufferMask[3]);
	//SetUseTextureToShader(14, ShaderDemoSpriteComponent::GetInstance()->GetZMaskHandle(3)); // Zマスクをシェーダーに設定
	SetUseVertexShader(_VShader[2]); // 頂点シェーダーを設定
	SetUsePixelShader(_PShader[2]); // ピクセルシェーダーを設定
	MV1DrawModel(_Model->GetHandle()); // モデルを描画
	MV1SetMeshBackCulling(_Model->GetHandle(), 0, DX_CULLING_LEFT);
	SetUseTextureToShader(13, -1);
	SetUseTextureToShader(14, -1);
}

void ShaderDemoSubSpriteComponent::DrawMask()
{

	VECTOR cTarget = GetCameraTarget();
	VECTOR cPos = GetCameraPosition();
	float nearZ = GetCameraNear();
	float farZ = GetCameraFar();

	SetDrawScreen(_ZBufferMask[3]); // 描画先をZバッファに設定

	ClearDrawScreen(); // 描画先をクリア

	SetRenderTargetToShader(1, -1); // Zバッファをシェーダーに設定
	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定
	SetUseVertexShader(_VShader[2]);
	SetUsePixelShader(_PShader[3]);
	MV1DrawModel(_Model->GetHandle()); // モデルを描画


	SetDrawScreen(_ZBufferMask[4]); // 描画先をZバッファに設定
	ClearDrawScreen(); // 描画先をクリア
	DrawGraph(0, 0, _ZBufferMask[1], TRUE); // バックカリング用の描画先を描画

	SetDrawScreen(_BackCullMask);
	ClearDrawScreen(); // 描画先をクリア



	SetUseZBufferFlag(TRUE); // Zバッファを使用する
	//MV1SetUseOrigShader(TRUE); // オリジナルのシェーダーを使用する

	SetUseVertexShader(_VShader[2]);
	SetUsePixelShader(_PShader[1]);

	SetDrawScreen(_ZBufferMask[1]); // 描画先をバックスクリーンに設定
	ClearDrawScreenZBuffer(); // Zバッファをクリア	

	//SetRenderTargetToShader(0, _ZBufferMask[1]);
	SetRenderTargetToShader(1, _BackCullMask); // Zバッファをシェーダーに設定
	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定

	SetUseTextureToShader(11, _ZBufferMask[3]); // Zマスクをシェーダーに設定
	SetUseTextureToShader(12, _ZBufferMask[4]); // バックカリングマスクをシェーダーに設定
	SetUseTextureToShader(13, _ZBufferMask[2]); // 法線マップをシェーダーに設定
	SetUseTextureToShader(14, _ZBufferMask[0]); // Zバッファをシェーダーに設定

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

	SetDrawScreen(_ZBufferMask[0]); // 描画先をバックスクリーンに設定
	SetRenderTargetToShader(1, -1);
	//ClearDrawScreen();
	//SetRenderTargetToShader(0, _ZBufferMask[0]);


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
	SetUseTextureToShader(13, _ZBufferMask[1]);
	SetUseTextureToShader(14, _BackCullMask); // 法線マップをシェーダーに設定
	MV1DrawModel(_Model->GetHandle());

	//SetZBufferCmpType(DX_CMP_LESS);
	SetDrawScreen(DX_SCREEN_BACK); // 描画先をバックスクリーンに設定
	ClearDrawScreen();
	SetRenderTargetToShader(0, -1);



	SetCameraPositionAndTargetAndUpVec(cPos, cTarget, VGet(0, 1, 0)); // カメラの位置と向きを設定
	SetCameraNearFar(nearZ, farZ); // カメラの近くと遠くの距離を設定

	SetUseTextureToShader(11, -1);
	SetUseTextureToShader(12, -1);
	SetUseTextureToShader(13, -1);
	SetUseTextureToShader(14, -1); // 法線マップをシェーダーに設定

}