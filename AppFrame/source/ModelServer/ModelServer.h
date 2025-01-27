#pragma once

#include <list>
#include <map>
#include "../ModeServer/ActorClass.h"



class ModelServer
{

	struct Model
	{
		std::list<int> handle;
		std::string filepass;
	};

public:
	ModelServer();
	virtual	~ModelServer();
	static ModelServer* _lpInstance;
	static ModelServer* GetInstance() { return (ModelServer*)_lpInstance; }

	int Add(const TCHAR *filename);		// “o˜^
	int Add(std::string filename);		// “o˜^‚Í‚·‚é‚ªAˆê“xƒƒCƒ“‚ğ‰ñ‚³‚È‚¢‚Æ‚¢‚¯‚È‚¢

	int Del(ModeBase* mode);		// íœ—\–ñ


private:
	//std::map<std::string, std::list<int>> _Models;
	std::list<Model> _Models;			// ƒ‚ƒfƒ‹ƒŠƒXƒg

};

