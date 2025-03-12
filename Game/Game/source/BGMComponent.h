#pragma once

#include "appframe.h"


class BGMComponent : public SoundComponent {
public:
	BGMComponent(ActorClass* owner, bool deadtype, std::string name1, std::string name2 = "");
	virtual ~BGMComponent();

	void Update() override;

private:
	class ModeGame* _Mode;
	int _Playnum;
};

