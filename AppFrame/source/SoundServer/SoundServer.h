#pragma once

#include <Windows.h>
#include <stdio.h>
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <string>
#include <map>
#include "WAVRead.h"
#include "../ModeServer/ActorClass.h"

class SoundServer {
public:
	SoundServer();
	virtual ~SoundServer();

	static SoundServer* _lpInstance;
	static SoundServer* GetInstance() { return (SoundServer*)_lpInstance; }

	void Clear();
	bool Add(std::string path,std::string name);
	void Create(class ActorClass* owner,std::string name);
	void Create(std::string name);
	bool Del(std::string name);


protected:
	bool Init();
	IXAudio2* _XAudio2;
	IXAudio2MasteringVoice* _MasteringVoice;

	std::map<std::string, WAVRead::WAVDATA> _m;

	// debug
	IXAudio2SourceVoice* sourceVoice;
	
};

