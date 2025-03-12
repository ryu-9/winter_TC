#pragma once

#include "SoundServer.h"
#include <vector>

class SourceVoiceItem;
class SoundComponent : public Component {
public:
	SoundComponent(ActorClass* owner,bool deadtype);
	virtual ~SoundComponent();


	void Update() override;

	void Play(int index);
	void SetSourceVoice(SourceVoiceItem* sv);
	void SetTimer(int time);
protected:
	std::vector<SourceVoiceItem*> _SV;

	int _Timer;
	bool _DeadType;
};

