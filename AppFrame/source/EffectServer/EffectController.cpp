#include "EffectController.h"


EffectController::EffectController(ModeBase* mode)
	:ActorClass(mode)
	,_Mode(mode)
{
	for (auto sp : mode->GetSprites()) {
		EffectManager* effect = new EffectManager(sp);
		_EffectList.emplace_back(effect);
	}
}

EffectController::~EffectController()
{
}

void EffectController::Draw()
{
	for (auto itr : _EffectList)
	{
		itr->Draw();
	}
}

void EffectController::AddEffect(EffectManager* effect)
{
	_EffectList.emplace_back(effect);
}

void EffectController::DelEffect(EffectManager* effect)
{
	for (int i = 0; _EffectList.size();i++)
	{
		if (_EffectList[i] == effect)
		{
			_EffectList.erase(_EffectList.begin() + i);
			break;
		}
	}
}

void EffectController::AddShadowMap(int size, VECTOR dir, VECTOR target, int index, float length, int num, int drawOrder)
{
	int tmp = num;
	if (num >= 0 && num < _ShadowMapList.size()) {
		delete _ShadowMapList[num];
			}
	else {
		num = _ShadowMapList.size();
		_ShadowMapList.emplace_back(nullptr);
	}
	_ShadowMapList[num] = new ShadowMapSpriteComponent(this, size, dir, target, index, length, drawOrder);


}


