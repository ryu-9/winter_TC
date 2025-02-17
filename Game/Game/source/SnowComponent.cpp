#include "SnowComponent.h"

SnowComponent::SnowComponent(ActorClass* owner, MV1_COLL_RESULT_POLY m)
	:SpriteComponent(owner)
{
	for (auto p : m.Position) {
		p = VAdd(p, VGet(0, 10, 0));
		VERTEX3D tmp;
		tmp.pos = p;
		tmp.norm = m.Normal;
		tmp.dif = GetColorU8(255, 255, 255, 255);
		tmp.spc = GetColorU8(0, 0, 0, 0);
		tmp.u = 0;
		tmp.v = 0;
		tmp.su = 1;
		tmp.sv = 1;
		_Snow.push_back(tmp);
		_Height.push_back(0);
	}
}

SnowComponent::~SnowComponent()
{
}

void SnowComponent::Draw()
{



	int debug = DrawPolygon3D(_Snow.data(), 1, DX_NONE_GRAPH, FALSE);
	return;

}
