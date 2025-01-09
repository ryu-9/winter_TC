#include "WAVRead.h"

HRESULT WAVRead::Read(const char* path, WAVDATA* wavData) {
	if (wavData == nullptr) return E_POINTER;
	
	HMMIO mmHandle = nullptr;
	
	mmHandle = mmioOpenA((LPSTR)path, nullptr, MMIO_READ);

	return S_OK;
}
