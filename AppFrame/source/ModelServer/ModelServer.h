#pragma once

#include <list>
#include "../ModeServer/ActorClass.h"



class ModelServer
{

	struct Model
	{
		std::list<int> handle;
		const TCHAR* filepass;
	};

public:
	ModelServer();
	virtual	~ModelServer();
	static ModelServer* _lpInstance;
	static ModelServer* GetInstance() { return (ModelServer*)_lpInstance; }

	int Add(const TCHAR *filename);		// 登録はするが、一度メインを回さないといけない
	int Del(ModeBase* mode);		// 削除予約


private:
	std::list<Model> _Models;			// モデルリスト

};

