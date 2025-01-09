#pragma once

#include "SoundServer.h"
class SoundComponent : public Component {
public:
	SoundComponent(class ActorClass* owner,int updateOrder = 100);
	virtual ~SoundComponent();

	void Update() override;

private:
	IXAudio2SourceVoice* _SourceVoice;
};

