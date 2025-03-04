#include "EffectController.h"


EffectController* EffectController::_lpInstance = nullptr;	

EffectController::EffectController(ModeBase* mode)
	:ActorClass(mode)
{
	_lpInstance = this;

}

EffectController::~EffectController()
{
}

void EffectController::Draw()
{
	//エフェクトの描画
	std::vector<int> flag;
	flag.resize(_EffectList.size());
	std::fill(flag.begin(), flag.end(), -1);

	auto sp = _Mode->GetSprites();
	std::map<std::string, bool> empty;
	for (int i = 0; i < sp.size(); i++) {
	
		auto iter = _FlagList.find(sp[i]);
		std::pair<SpriteComponent* , std::map<std::string, bool>> obj;

		if (iter == _FlagList.end()) {
			obj.second = empty;
		}
		else {
			obj = *iter;
		}


		for (int j = 0; j < _EffectList.size(); j++) {
			bool useflag;
			auto effiter = obj.second.find(_EffectList[j]->GetEffectName());
			if (effiter == obj.second.end()) {
				useflag = _EffectList[j]->GetIsUse();
			}
			else {
				useflag = effiter->second;
			}

			if (flag[j] != useflag) {
				_EffectList[j]->SetIsUse(useflag);
				flag[j] = useflag;
			}
		}
		sp[i]->Draw();
	}

}

void EffectController::AddEffect(EffectManager* effect)
{
	_EffectList.insert(_EffectList.begin(), effect);
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


void EffectController::AddEffectFlag(SpriteComponent* sprite, std::string flagname, bool flag)
{

	_FlagList[sprite][flagname] = flag;

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
	:EffectManager(owner, draworder)
	,_Color(color)
	,_MinDist(mindist)
	,_MaxDist(maxdist)

{
	SetEffectName("Fog");
}

FogSpriteComponent::~FogSpriteComponent()
{
}

void FogSpriteComponent::Draw()
{

}

void FogSpriteComponent::SetIsUse(bool flag)
{
	SetFogEnable(flag);					// フォグを有効にする
	int r, g, b;
	GetColor2(_Color, &r, &g, &b);
	SetFogColor(r, g, b);	// フォグの色を設定
	SetFogStartEnd(_MinDist, _MaxDist);	// フォグの開始と終了距離を設定
}

