#include "ModelServer.h"
#include "../AppFrame/source/ModeServer/EffectSpriteComponent.h"
#include <EffekseerForDXLib.h>

ModelServer* ModelServer::_lpInstance = NULL;


ModelServer::ModelServer()
:_UpdatedEffectNum(0)
{
	_lpInstance = this;
}

ModelServer::~ModelServer()
{
}

int ModelServer::Add(const TCHAR *filename)
{
	int handle;
	for (auto&model : _Models) {
		if (model.filepass == filename) {
			handle = MV1DuplicateModel(*model.handle.begin());
			model.handle.emplace_back(handle);
			return handle;
		}
	}
	_Models.emplace_back();
	SetUseASyncLoadFlag(TRUE);
	handle = MV1LoadModel(filename);
	SetUseASyncLoadFlag(FALSE);
	_Models.back().filepass = filename;
	_Models.back().handle.emplace_back(handle);

	return handle;
}

int ModelServer::AddGraph(const TCHAR* filename)
{
	int handle;
	for (auto& model : _Graph) {
		if (model.filepass == filename) {
			handle = model.handle[0];
			return handle;
		}
	}
	_Graph.emplace_back();
	handle = LoadGraph(filename);
	_Graph.back().filepass = filename;
	_Graph.back().handle.emplace_back(handle);

	return handle;
}

int ModelServer::AddEffect(const char* filename, float size)
{
	int handle;
	for (auto& model : _Effect) {
		if (model.filepass == filename
			) {
			handle = model.handle[0];
			return handle;
		}
	}
	_Effect.emplace_back();
	handle = LoadEffekseerEffect(filename, size);
	_Effect.back().filepass = filename;
	_Effect.back().handle.emplace_back(handle);

	return handle;
}

void ModelServer::DelEffectList(int handle)
{
	auto it = _EffectHandle.find(handle);
	if (it != _EffectHandle.end()) {
		_EffectHandle.erase(it);
	}
}

bool ModelServer::AddEffectList(int handle, SpriteComponent* sp)
{
	auto it = _EffectHandle.find(handle);
	if (it != _EffectHandle.end()) {
		auto es = dynamic_cast<EffectSpriteComponent*>(it->second);
		if (es->GetLoopFlag()) {
			es->Play();
		}
		else
		{
			es->SetHandle(-99999);
			//delete it->second;
		}
		return false;
	}
	_EffectHandle[handle] = sp;
	return true;
}

void ModelServer::UpdateEffect()
{
	_UpdatedEffectNum++;
	if (_UpdatedEffectNum >= _EffectHandle.size() && _EffectHandle.size())
	{
		_UpdatedEffectNum = 0;
		UpdateEffekseer3D();
		DrawEffekseer3D();
	}
}



int ModelServer::Del(ModeBase* mode)
{
	return 0;
}
