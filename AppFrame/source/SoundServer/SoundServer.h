#pragma once
#include "SoundItem.h"

#include <Windows.h>
#include <stdio.h>
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <string>
#include <map>

class SoundServer {
public:
	SoundServer();
	virtual ~SoundServer();

	void Clear();
	void Add(std::string path);
	bool Del(std::string name);

	struct WAVEDATA {
		WAVEFORMATEX wFormat;
		char* sBuffer;
		DWORD size;

		~WAVEDATA() { free(sBuffer); }
	};
protected:
	bool Init();
	IXAudio2* _XAudio2;
	IXAudio2MasteringVoice* _MasteringVoice;

	std::map < std::string, WAVEDATA> _mSounds;
};

