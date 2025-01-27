#include "ModelServer.h"

/// インスタンス 
ModelServer* ModelServer::_lpInstance = NULL;


ModelServer::ModelServer()
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

int ModelServer::Add(std::string filename)
{
	int handle;
	for (auto& model : _Models) {
		if (model.filepass == filename) {
			handle = MV1DuplicateModel(*model.handle.begin());
			model.handle.emplace_back(handle);
			return handle;
		}
	}
	_Models.emplace_back();
	handle = MV1LoadModel(filename.c_str());
	_Models.back().filepass = filename;
	_Models.back().handle.emplace_back(handle);

	return handle;
}

int ModelServer::Del(ModeBase* mode)
{
	return 0;
}
