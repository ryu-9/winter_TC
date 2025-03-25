
#include "ApplicationBase.h"
#include <vector>
#include <EffekseerForDXLib.h>

ApplicationBase	*ApplicationBase::_lpInstance = NULL;

ApplicationBase::ApplicationBase() {
	_lpInstance = this;
}

ApplicationBase::~ApplicationBase() {
}

bool ApplicationBase::Initialize(HINSTANCE hInstance) {

	// DXライブラリの初期化
	if (AppWindowed()) {
		ChangeWindowMode(true);							// ウィンドウモードに指定する
	}
	SetGraphMode(DispSizeW(), DispSizeH(), 32);

	SetZBufferBitDepth(32);	// Zバッファのビット深度を24bitに設定

	
	SetUseDirect3DVersion(DX_DIRECT3D_11);	// Direct3D11を使用する
	
	if (DxLib_Init() == -1)
	{	// エラーが起きたら直ちに終了
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);		// 描画先画面を裏画面にセット

	if (Effekseer_Init(8000) == -1) {
		return false;
	}

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// 乱数初期化
	srand((unsigned int)time(NULL));
	
	// モードサーバの初期化
	_serverMode = new ModeServer();
	_serverModel = new ModelServer();
	_serverSound = new SoundServer();

	return true;
}

bool ApplicationBase::Terminate() {
	// DXライブラリ開放
	Effkseer_End();
	DxLib_End();

	return true;
}


bool ApplicationBase::Input() {
	// キーの入力、トリガ入力を得る
	{
		int keyold = _gKey[0];
		_gKey[0] = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		_gTrg[0] = (_gKey[0] ^ keyold) & _gKey[0];	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）
		_gRel[0] = (_gKey[0] ^ keyold) & keyold;	// キーのリリース情報生成（離した瞬間しか反応しないキー情報）
	}

	{
		int keyold = _gKey[1];
		_gKey[1] = GetJoypadInputState(DX_INPUT_PAD2);
		_gTrg[1] = (_gKey[1] ^ keyold) & _gKey[1];	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）
		_gRel[1] = (_gKey[1] ^ keyold) & keyold;	// キーのリリース情報生成（離した瞬間しか反応しないキー情報）
	}
	
	return true;
}

bool ApplicationBase::Process() {
	_serverMode->ProcessInit();
	_serverMode->Process();
	_serverMode->ProcessFinish();
	SoundServer::GetInstance()->UpdateDeleteSV();
	return true;
}

bool ApplicationBase::Update()
{
	return false;
}

bool ApplicationBase::Render() {
	_serverMode->RenderInit();
	_serverMode->Render();
	_serverMode->RenderFinish();
	return true;
}
