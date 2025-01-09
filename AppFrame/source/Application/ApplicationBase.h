
#include "dxlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../ModeServer/ModeServer.h"
#include "../ModelServer/ModelServer.h"
#include "../SoundServer/SoundServer.h"

class ApplicationBase
{
public:
	ApplicationBase();
	virtual ~ApplicationBase();

	virtual bool Initialize(HINSTANCE hInstance);
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Update();
	virtual bool Render();

	virtual bool AppWindowed() { return true; }
	virtual int DispSizeW() { return 640; }
	virtual int DispSizeH() { return 480; }

	static	ApplicationBase	*GetInstance() { return _lpInstance; }
	virtual int GetKey() { return _gKey; }
	virtual int GetTrg() { return _gTrg; }
	virtual int GetRel() { return _gRel; }

protected:
	static	ApplicationBase	*_lpInstance;

	int		_gKey, _gTrg, _gRel;

	ModeServer* _serverMode;
	ModelServer* _serverModel;
	SoundServer* _serverSound;

};
