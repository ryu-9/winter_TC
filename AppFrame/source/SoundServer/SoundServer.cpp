#include "SoundServer.h"
#include <mmsystem.h>
#include "SoundComponent.h"
#include "SourceVoiceItem.h"
#include "SourceVoiceItemEffect.h"
#include <xapofx.h>
#include "xaudio2fx.h"
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
		if (_WavData.count(name)) {
			_WavData.erase(name);
		}
		
		if (WAVRead::Read(path.c_str(), &_WavData[name]) == false) {
			return false;
		}
	}
	return true;
}

SourceVoiceItem* SoundServer::Create(ActorClass* p, std::string wavname, std::string dataname,std::string mapname, int startTime , int loopflag) {
	if (_WavData.count(wavname) == 0) { return nullptr; }
	
	IXAudio2SourceVoice* sourceVoice = nullptr;
	HRESULT hr = _XAudio2->CreateSourceVoice(&sourceVoice, &_WavData[wavname].wFormat, XAUDIO2_VOICE_USEFILTER, 16.0f);
	if (FAILED(hr)) {
		printf("CreateSourceVoice failed: %#X\n", hr);
		return nullptr;
	}

	// 音声データの設定
	XAUDIO2_BUFFER xAudio2Buffer{};
	xAudio2Buffer.pAudioData = (BYTE*)_WavData[wavname].sBuffer;
	if (loopflag) {
		xAudio2Buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	} 
	if (startTime != 0) {
		xAudio2Buffer.PlayBegin = startTime;
	}
	xAudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
	xAudio2Buffer.AudioBytes = _WavData[wavname].size;

	sourceVoice->SubmitSourceBuffer(&xAudio2Buffer);

	// ソースボイスの作成
	auto sv = new SourceVoiceItem(mapname,p);
	sv->SetSourceVoice(sourceVoice);
	
	sourceVoice->SetVolume(1.0f);
	sourceVoice->SetFrequencyRatio(1.0f);
	if(dataname == "AttackSE"){
		auto p = new SVItemPitchRand(sv);
		p->SetUp();
	}
	if (dataname == "TDX") {
		IUnknown* xapo;
		XAudio2CreateReverb(&xapo);

		XAUDIO2_EFFECT_DESCRIPTOR descriptor;
		descriptor.InitialState = TRUE;
		descriptor.OutputChannels = 2;
		descriptor.pEffect = xapo;

		XAUDIO2_EFFECT_CHAIN chain;
		chain.EffectCount = 1;
		chain.pEffectDescriptors = &descriptor;

		sourceVoice->SetEffectChain(&chain);

		xapo->Release();
	}
	
	_SV[p][mapname] = sv;
	
	return sv;
}
;



void SoundServer::Update(ActorClass* p) {
	std::vector<std::string> deleteList;
	for (auto& sv : _SV[p]) {
		if (sv.second->IsToDestroy()) {
			deleteList.push_back(sv.first);

		}
	}
	for (auto& name : deleteList) {
		_SV[p].erase(name);
		if (_SV[p].size() == 0) {
			_SV.erase(p);
		}
	}
	for (auto& sv : _SV[p]) {
		sv.second->Update(p);
	}
}

void SoundServer::UpdateDeleteSV() {
	std::vector<SourceVoiceItem*> deleteSV;
	for (auto i = 0; i < _DeleteSV.size(); i++) {
		if (!_DeleteSV[i]->IsPlay()) {
			deleteSV.push_back(_DeleteSV[i]);
			_DeleteSV.erase(_DeleteSV.begin() + i);
		}
	}
	for (auto i = 0; i < deleteSV.size(); i++) {
		delete deleteSV[i];
	}
	for (auto i = 0; i < _DeleteSV.size(); i++) {
		_DeleteSV[i]->Update(nullptr);
	}
}

void SoundServer::Release(ActorClass* p) {
	for (auto& sv : _SV[p]) {
		sv.second->Stop();
		_DeleteSV.push_back(sv.second);
	}
	_SV.erase(p);
}

void SoundServer::DeleteSourceVoice(ActorClass* p, std::string name) {
	if (_SV.count(p) == 0) { return; }
	if (_SV[p].count(name) == 0) { return; }
	_SV[p][name]->Stop();
	_DeleteSV.push_back(_SV[p][name]);
	_SV[p].erase(name);
	if (_SV[p].size() == 0) {
		_SV.erase(p);
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

SourceVoiceItem* SoundServer::GetSourceVoice(ActorClass* p, std::string name) {
	if (_SV.count(p) == 0) { return nullptr; }
	if (_SV[p].count(name) == 0) { return nullptr; }
	return _SV[p][name];
}