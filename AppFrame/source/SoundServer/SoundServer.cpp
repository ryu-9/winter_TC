#include "SoundServer.h"
#include <mmsystem.h>
#include "SEComponent.h"
#include "SoundComponent.h"
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


bool SoundServer::Add(std::string path,std::string name, bool isoverwrite ) {
	// サウンドの読み込み
	if (_WavData.count(name)&& !isoverwrite) { return true; }	
	else {
		if (WAVRead::Read(path.c_str(), &_WavData[name]) == false) {
			return false;
		}
	}
	return true;
}

bool SoundServer::Create(SEComponent* secon, std::string name) {
	auto sv = new SourceVoiceItem();
	secon->SetSourceVoice(sv);
	return true;
}


bool SoundServer::Create(std::string name, IXAudio2SourceVoice*& sv,int hz,int loop) {
	IXAudio2SourceVoice* sourceVoice = nullptr;
	if (_WavData.count(name) == 0) { return false; }
	HRESULT hr = _XAudio2->CreateSourceVoice(&sourceVoice, &_WavData[name].wFormat,XAUDIO2_VOICE_USEFILTER,16.0f);
	if (FAILED(hr)) {
		printf("CreateSourceVoice failed: %#X\n", hr);
		return false;
	}

	// 音声データの設定
	XAUDIO2_BUFFER xAudio2Buffer{};
	xAudio2Buffer.pAudioData = (BYTE*)_WavData[name].sBuffer;
	xAudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
	xAudio2Buffer.AudioBytes = _WavData[name].size;
	
	xAudio2Buffer.PlayBegin = hz;
	xAudio2Buffer.LoopCount = loop;
	sourceVoice->SubmitSourceBuffer(&xAudio2Buffer);
	sv = sourceVoice;
	return true;
}

bool SoundServer::Create(ActorClass* p, std::string wavname, std::string dataname,std::string mapname) {
	if (_WavData.count(wavname) == 0) { return false; }
	
	IXAudio2SourceVoice* sourceVoice = nullptr;
	HRESULT hr = _XAudio2->CreateSourceVoice(&sourceVoice, &_WavData[dataname].wFormat, XAUDIO2_VOICE_USEFILTER, 16.0f);
	if (FAILED(hr)) {
		printf("CreateSourceVoice failed: %#X\n", hr);
		return false;
	}

	// 音声データの設定
	XAUDIO2_BUFFER xAudio2Buffer{};
	xAudio2Buffer.pAudioData = (BYTE*)_WavData[wavname].sBuffer;
	xAudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
	xAudio2Buffer.AudioBytes = _WavData[wavname].size;

	sourceVoice->SubmitSourceBuffer(&xAudio2Buffer);

	// ソースボイスの作成
	auto sv = new SourceVoiceItem();
	sv->SetSourceVoice(sourceVoice);
	
	_SV[p][mapname] = sv;
	
	return true;
}
;



void SoundServer::UpdateSound(ActorClass* p) {
	for (auto& sv : _SV[p]) {
		sv.second->Update();
	}
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
