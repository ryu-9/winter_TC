#pragma once

#include "appframe.h"


class BGMComponent : public SoundComponent {
public:
	BGMComponent(ActorClass* owner);
	virtual ~BGMComponent();

	void Update() override;

};

