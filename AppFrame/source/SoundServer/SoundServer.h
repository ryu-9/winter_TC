#pragma once

#include <Windows.h>
#include <stdio.h>
#define XAUDIO2_HELPER_FUNCTIONS
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <string>
#include <map>
#include "WAVRead.h"
#include "../ModeServer/ActorClass.h"

class SourceVoiceItem;

class SoundServer {
public:
	SoundServer();
	virtual ~SoundServer();

	static SoundServer* _lpInstance;
	static SoundServer* GetInstance() { return (SoundServer*)_lpInstance; }



	// ÉTÉEÉìÉhê∂ê¨ån
	bool Add(std::string path,std::string name,bool isoverwrite = false);
	bool Create(class SEComponent* owner, std::string name);
	bool Create(std::string name,IXAudio2SourceVoice*& sv,int hz = 0,int loop = 0);

	bool Create(ActorClass* p, std::string name, std::string dataname , std::string mapname);

	SourceVoiceItem* GetSourceVoice(ActorClass* p, std::string name) { return _SV[p][name]; }

	void UpdateSound(ActorClass* p);

protected:
	bool Init();
	IXAudio2* _XAudio2;
	IXAudio2MasteringVoice* _MasteringVoice;

	std::map<std::string, WAVRead::WAVDATA> _WavData;
	std::map<ActorClass*, std::map<std::string,SourceVoiceItem*>> _SV;
	
};

