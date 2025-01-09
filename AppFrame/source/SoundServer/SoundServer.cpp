#include "SoundServer.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


// インスタンス
SoundServer* SoundServer::_lpInstance = nullptr;


SoundServer::SoundServer() 
	: _XAudio2(nullptr), _MasteringVoice(nullptr)
{
	_lpInstance = this;
	if (Init() == FALSE){
		delete this;
	}
	
}

SoundServer::~SoundServer() {
	// マスターボイスの解放
	if (_MasteringVoice) {
		_MasteringVoice->DestroyVoice();
		_MasteringVoice = nullptr;
	}
	// XAudio2の解放
	if (_XAudio2) {
		_XAudio2->Release();
		_XAudio2 = nullptr;
	}
	// COMの終了処理
	CoUninitialize();
}

void SoundServer::Clear()
{
}

void SoundServer::Add(std::string path,std::string name) {
	if (WAVRead::Read(path.c_str(), &wavData) == false) {
		return;
	}

	// ソースボイスの作成
	sourceVoice = nullptr;

	HRESULT hr = _XAudio2->CreateSourceVoice(&sourceVoice, &wavData.wFormat);
	if (FAILED(hr)) {
		printf("CreateSourceVoice failed: %#X\n", hr);
		return;
	}

	// デバッグ用
	XAUDIO2_BUFFER xAudio2Buffer{};
	xAudio2Buffer.pAudioData = (BYTE*)wavData.sBuffer;
	xAudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
	xAudio2Buffer.AudioBytes = wavData.size;

	xAudio2Buffer.LoopCount = 0;
	sourceVoice->SubmitSourceBuffer(&xAudio2Buffer);

	// 実際に音を鳴らす
	sourceVoice->Start();

	return ;
}

bool SoundServer::Del(std::string name)
{
	return false;
}

bool SoundServer::Init() {
	// XAudio2の初期化
	

	UINT32 flags = 0;
	HRESULT hr = XAudio2Create(&_XAudio2, flags);
	// 例外処理
	if (FAILED(hr)) {
		printf("XAudio2Create failed: %#X\n", hr);
		CoUninitialize();
		return false;
	}

	// デバッグ設定(windows8以降)
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
	XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
	debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	_XAudio2->SetDebugConfiguration(&debug, 0);
#endif

	// マスターボイスの作成
	if (FAILED(hr = _XAudio2->CreateMasteringVoice(&_MasteringVoice))) {
		printf("CreateMasteringVoice failed: %#X\n", hr);
		_XAudio2->Release(); _XAudio2 = nullptr;
		CoUninitialize();
		return false;
	}

	return true;
}
