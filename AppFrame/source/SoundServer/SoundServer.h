#pragma once

#include <Windows.h>
#include <stdio.h>
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <string>
#include <map>
#include "WAVRead.h"

class SoundServer {
public:
	SoundServer();
	virtual ~SoundServer();

	static SoundServer* _lpInstance;
	static SoundServer* GetInstance() { return (SoundServer*)_lpInstance; }

	void Clear();
	void Add(std::string path);
	bool Del(std::string name);


protected:
	bool Init();
	IXAudio2* _XAudio2;
	IXAudio2MasteringVoice* _MasteringVoice;

	// debug
	IXAudio2SourceVoice* sourceVoice;
	WAVRead::WAVDATA wavData;
};

