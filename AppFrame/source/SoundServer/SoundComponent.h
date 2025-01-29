#pragma once

#include "SourceVoiceItem.h"
#include "SoundServer.h"
#include <vector>

class SoundComponent : public Component {
public:
	SoundComponent(ActorClass* owner);
	virtual ~SoundComponent();


	void Update() override;

	void SetSourceVoice(SourceVoiceItem* sv);

protected:
	std::vector<SourceVoiceItem*> _SV;
};

