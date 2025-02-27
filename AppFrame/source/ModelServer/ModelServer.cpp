#include "ModelServer.h"
#include <EffekseerForDXLib.h>

/// インスタンス 
ModelServer* ModelServer::_lpInstance = NULL;


ModelServer::ModelServer()
: _EffectNum(0)
,_UpdatedEffectNum(0)
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
	handle = MV1LoadModel(filename);
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
	_EffectNum++;
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

void ModelServer::DelEffect(int handle)
{
	_EffectNum--;
}

void ModelServer::UpdateEffect()
{
	_UpdatedEffectNum++;
	if (_UpdatedEffectNum >= _EffectNum && _EffectNum)
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
