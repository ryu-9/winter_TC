#include "EffectController.h"


EffectController* EffectController::_lpInstance = nullptr;	

EffectController::EffectController(ModeBase* mode)
	:ActorClass(mode)
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
	//エフェクトの描画
	auto sp = _Mode->GetSprites();
	for (int i = 0; i < sp.size(); i++)
	{
		auto iter = _FlagList.find(sp[i]);
		if (iter == _FlagList.end()) {
			for(int j = 0; j < _EffectList.size(); j++) {
				_EffectList[i]->SetIsUse(_EffectList[i]->GetIsUse());
			}
		}
		else {
		
		}
		bool flag;

		for (int j = 0; j < _EffectList.size(); j++) {
			auto iter = _FlagList.find(sp[i]);
			if (iter == _FlagList.end()) {
				flag = _EffectList[j]->GetIsUse();
			}
			else {
				auto iter2 = iter->second.find("ShadowMap" + std::to_string(j));
				if (iter2 == iter->second.end()) {
					flag = _ShadowMapList[j]->GetIsDraw();
				}
				else {
					flag = iter2->second;
				}
			}
			if (flag) {
				SetUseShadowMap(j, _ShadowMapList[j]->GetHandle());
			}
		}
		sp[i]->Draw();
	}

	/*
		auto sp = _Mode->GetSprites();
	for (int i = 0; i < sp.size(); i++)
	{
		if (i < _EffectList.size()) {
			for (int j = 0; j < _ShadowMapFlagList.size(); j++) {
				bool flag = SearchFlag(_EffectList[i]->GetEffectFlag(), "ShadowMap" + std::to_string(j));
				if (_ShadowMapFlagList[j] != flag) {
					SetShadowMapUse(j, flag);
				}
			}
		
		}
		sp[i]->Draw();
	}
	*/

}

void EffectController::AddEffect(EffectManager* effect)
{
	int myDrawOrder = effect->GetSprite()->GetDrawOrder();
	auto iter = _EffectList.begin();
	for (; iter != _EffectList.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetSprite()->GetDrawOrder()) { break; }
	}

	_EffectList.insert(iter, effect);
}

void EffectController::DelEffect(EffectManager* effect)
{
	for (int i = 0; i < _EffectList.size();i++)
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
		_ShadowMapFlagList.emplace_back(true);
	}
	_ShadowMapList[tmp] = new ShadowMapSpriteComponent(this, size, dir, target, tmp, length, drawOrder);


}

void EffectController::SetShadowMapUse(int num, bool flag)
{
	if (num >= _ShadowMapFlagList.size()) {return;}

	_ShadowMapFlagList[num] = flag;
	if (flag) { SetUseShadowMap(num, _ShadowMapList[num]->GetHandle()); }
	else { SetUseShadowMap(num, -1); }
}

void EffectController::SetFlag(SpriteComponent* sp, std::string flagname, bool flag)
{
	for (auto effect : _EffectList)
	{
		if (effect->GetSprite() == sp)
		{
			effect->AddEffectFlag(flagname, flag);
			break;
		}
	}
}

void EffectController::AddEmphasisEffect(SpriteComponent* sprite, int alpha, int wide, int height, int draworder)
{
	PlayerEmphasisEffect* effect = new PlayerEmphasisEffect(this, sprite, alpha, wide, height, draworder);
	_EmphasisEffectList.emplace_back(effect);
}

bool EffectController::SearchFlag(std::map<std::string, bool>* flagList, std::string flagname)
{
	auto iter = flagList->find(flagname);
	if (iter == flagList->end()) {
		return false;
	}
	return iter->second;
}



FogSpriteComponent::FogSpriteComponent(ActorClass* owner, int draworder, unsigned int color, float mindist, float maxdist)
	:SpriteComponent(owner, draworder)
	,_Color(color)
	,_MinDist(mindist)
	,_MaxDist(maxdist)
	,_IsDraw(TRUE)
{
}

FogSpriteComponent::~FogSpriteComponent()
{
}

void FogSpriteComponent::Draw()
{
	SetFogEnable(TRUE);					// フォグを有効にする
	SetFogColor(190, 200, 255);			// フォグの色
	SetFogStartEnd(1000.0f, 6000.0f);	// フォグの適用距離
}

