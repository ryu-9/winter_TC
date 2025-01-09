#pragma once
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <stdint.h>

namespace WAVRead {
	struct WAVDATA {
		WAVEFORMATEX wFormat;
		char* sBuffer;
		uint32_t size;

		~WAVDATA() { free(sBuffer); }
	};

	bool Read(const char* path, WAVDATA* wavData);

}

