#pragma once

#include "SoundServer.h"
class SoundComponent : public Component {
public:
	SoundComponent(class ActorClass* owner, IXAudio2SourceVoice* sv,int updateOrder = 100);
	virtual ~SoundComponent();

	void Update() override;

	void Receive(int message) override;
private:


	IXAudio2SourceVoice* _SourceVoice;
};

