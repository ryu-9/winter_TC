#pragma once
#include "appframe.h"

class SnowComponent : public SpriteComponent
{
public:
	SnowComponent(class ActorClass* owner, MV1_COLL_RESULT_POLY m);
	virtual ~SnowComponent();
	void Draw() override;

private:
	std::vector<VERTEX3D> _Snow;
	std::deque<float> _Height;
	VECTOR _Normal;

};

