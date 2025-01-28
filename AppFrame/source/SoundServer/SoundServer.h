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
#include "SEComponent.h"

class SoundServer {
public:
	SoundServer();
	virtual ~SoundServer();

	static SoundServer* _lpInstance;
	static SoundServer* GetInstance() { return (SoundServer*)_lpInstance; }

	void Clear();
	bool Add(std::string path,std::string name);
	bool Create(class SEComponent* owner, std::string name);
	bool Create(class ActorClass* owner,std::string name,int seflag);
	bool Create(std::string name,IXAudio2SourceVoice* sv);
	bool Del(std::string name);


protected:
	bool Init();
	IXAudio2* _XAudio2;
	IXAudio2MasteringVoice* _MasteringVoice;

	std::map<std::string, WAVRead::WAVDATA> _m;
	
};

