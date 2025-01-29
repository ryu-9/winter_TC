#pragma once

#include "SoundServer.h"
#include <vector>

class SourceVoiceItem;
class SoundComponent : public Component {
public:
	SoundComponent(ActorClass* owner);
	virtual ~SoundComponent();


	void Update() override;

	void SetSourceVoice(SourceVoiceItem* sv);

protected:
	std::vector<SourceVoiceItem*> _SV;
};

