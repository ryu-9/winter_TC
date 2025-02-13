#include "EffectController.h"


EffectController* EffectController::_lpInstance = nullptr;	

EffectController::EffectController(ModeBase* mode)
	:ActorClass(mode)
	,_Mode(mode)
{
	_lpInstance = this;

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
	for (auto itr : _Mode->GetSprites())
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

void EffectController::AddShadowMap(int size, VECTOR dir, VECTOR target, int index, float length, int drawOrder)
{
	int tmp = index;
	if (tmp >= 0 && tmp < _ShadowMapList.size()) {
		delete _ShadowMapList[tmp];
			}
	else {
		tmp = _ShadowMapList.size();
		_ShadowMapList.emplace_back(nullptr);
	}
	_ShadowMapList[tmp] = new ShadowMapSpriteComponent(this, size, dir, target, tmp, length, drawOrder);


}


