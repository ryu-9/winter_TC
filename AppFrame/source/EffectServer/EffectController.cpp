// EffectController.cpp
#include "EffectController.h"

// シングルトンインスタンスの初期化
EffectController* EffectController::_lpInstance = nullptr;	

// コンストラクタ
EffectController::EffectController(ModeBase* mode)
	:ActorClass(mode)
{
	_lpInstance = this;
}

// デストラクタ
EffectController::~EffectController()
{
}

// エフェクトの描画を行う関数
void EffectController::Draw()
{
	// エフェクトのフラグを初期化
	std::vector<int> flag;
	flag.resize(_EffectList.size());
	std::fill(flag.begin(), flag.end(), -1);

	// スプライトを取得
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

		// 各エフェクトの使用フラグを設定
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
		// スプライトを描画
		sp[i]->Draw();
	}
}

// エフェクトを追加する関数
void EffectController::AddEffect(EffectManager* effect)
{
	_EffectList.insert(_EffectList.begin(), effect);
}

// エフェクトを削除する関数
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

// エフェクトフラグを追加する関数
void EffectController::AddEffectFlag(SpriteComponent* sprite, std::string flagname, bool flag)
{
	_FlagList[sprite][flagname] = flag;
}

// 強調エフェクトを追加する関数
void EffectController::AddEmphasisEffect(SpriteComponent* sprite, int alpha, int wide, int height, int draworder)
{
	PlayerEmphasisEffect* effect = new PlayerEmphasisEffect(this, sprite, alpha, wide, height, draworder);
	_EmphasisEffectList.emplace_back(effect);
}

// フラグを検索する関数
bool EffectController::SearchFlag(std::map<std::string, bool>* flagList, std::string flagname)
{
	auto iter = flagList->find(flagname);
	if (iter == flagList->end()) {
		return false;
	}
	return iter->second;
}

// FogSpriteComponentのコンストラクタ
FogSpriteComponent::FogSpriteComponent(ActorClass* owner, int draworder, unsigned int color, float mindist, float maxdist)
	:EffectManager(owner, draworder)
	,_Color(color)
	,_MinDist(mindist)
	,_MaxDist(maxdist)
{
	SetEffectName("Fog");
}

// FogSpriteComponentのデストラクタ
FogSpriteComponent::~FogSpriteComponent()
{
}

// FogSpriteComponentの描画関数
void FogSpriteComponent::Draw()
{
	// 何もしない
}

// FogSpriteComponentの使用フラグを設定する関数
void FogSpriteComponent::SetIsUse(bool flag)
{
	SetFogEnable(flag);					// フォグを有効にする
	int r, g, b;
	GetColor2(_Color, &r, &g, &b);
	SetFogColor(r, g, b);	// フォグの色を設定
	SetFogStartEnd(_MinDist, _MaxDist);	// フォグの開始と終了距離を設定
}
