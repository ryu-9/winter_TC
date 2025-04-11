#pragma once
#include "appframe.h"
class BGMActor : public ActorClass{
	typedef SoundServer SS;
public:
	BGMActor(ModeBase* mode);
	virtual ~BGMActor();

	void UpdateActor() override;

private:
	bool _SetFilter = false;	// フィルター適用中か
	int _PlayBGM;				// BGMのID
};

