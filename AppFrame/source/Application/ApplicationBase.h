
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
	virtual int GetKey(int n = 1) { return _gKey[n-1]; }
	virtual int GetTrg(int n = 1) { return _gTrg[n-1]; }
	virtual int GetRel(int n = 1) { return _gRel[n-1]; }

protected:
	static	ApplicationBase	*_lpInstance;

	int		_gKey[2], _gTrg[2], _gRel[2];

	ModeServer* _serverMode;
	ModelServer* _serverModel;
	SoundServer* _serverSound;

};
