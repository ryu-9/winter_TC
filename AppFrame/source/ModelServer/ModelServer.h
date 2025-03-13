#pragma once

#include <list>
#include <map>
#include "../ModeServer/ActorClass.h"



class ModelServer
{

	struct Model
	{
		std::vector<int> handle;
		std::string filepass;
	};

public:
	ModelServer();
	virtual	~ModelServer();
	static ModelServer* _lpInstance;
	static ModelServer* GetInstance() { return (ModelServer*)_lpInstance; }

	int Add(const TCHAR *filename);		// 登録
	
	int AddGraph(const TCHAR* filename);		// 登録

	int AddEffect(const char* filename, float size = 1.0f);		// 登録
	void DelEffectList(int handle);		// 削除
	bool AddEffectList(int handle, SpriteComponent* sp);		// 追加
	void UpdateEffect();		// 更新

	int Del(ModeBase* mode);		// 削除予約


private:
	//std::map<std::string, std::list<int>> _Models;
	std::list<Model> _Models;			// モデルリスト
	std::list<Model> _Graph;			// グラフィックリスト
	std::list<Model> _Effect;			// エフェクトリスト
	std::map<int, SpriteComponent*> _EffectHandle;		// エフェクトハンドルリスト
	int _UpdatedEffectNum;
};

