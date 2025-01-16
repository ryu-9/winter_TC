#include "SourceVoiceItem.h"

SourceVoiceItem::SourceVoiceItem(std::string wavname)
	:_SV(nullptr)
	, _Volume(1.0f)
{
	if (SoundServer::GetInstance()->Create(wavname, _SV) == false) {
		printf("CreateSourceVoice failed\n");
		delete this;
	}

}

SourceVoiceItem::~SourceVoiceItem() {
	if (_SV) {
		_SV->DestroyVoice();
		_SV = nullptr;
	}
}

void SourceVoiceItem::Play() {
	_SV->Start();
}

void SourceVoiceItem::Stop() {
	
}




