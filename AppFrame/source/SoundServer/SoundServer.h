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

	SourceVoiceItem* Create(ActorClass* p, std::string name, std::string dataname , std::string mapname, int startTime = 0, int loopflag = false);

	SourceVoiceItem* GetSourceVoice(ActorClass* p, std::string name);

	void Update(ActorClass* p);
	void UpdateDeleteSV();

	void Release(ActorClass* p);
	void DeleteSourceVoice(ActorClass* p, std::string name);

	IXAudio2MasteringVoice* GetMasteringVoice() { return _MasteringVoice; }

protected:
	bool Init();
	IXAudio2* _XAudio2;
	IXAudio2MasteringVoice* _MasteringVoice;

	std::map<std::string, WAVRead::WAVDATA> _WavData;
	std::map<ActorClass*, std::map<std::string,SourceVoiceItem*>> _SV;
	std::vector<SourceVoiceItem*> _DeleteSV;
	
};

