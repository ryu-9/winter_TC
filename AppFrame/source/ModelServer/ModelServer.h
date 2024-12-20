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

	int Add(const TCHAR *filename);		// �o�^�͂��邪�A��x���C�����񂳂Ȃ��Ƃ����Ȃ�
	int Del(ModeBase* mode);		// �폜�\��


private:
	std::list<Model> _Models;			// ���f�����X�g

};

