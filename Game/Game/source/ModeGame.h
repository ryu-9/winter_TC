#pragma once
#include "appframe.h"

#include <string>
#include "ModeMenu.h"

// 
#define	PI	(3.1415926535897932386f)
#define	DEG2RAD(x)			( ((x) / 180.0f ) * PI )
#define	RAD2DEG(x)			( ((x) * 180.0f ) / PI )


class ModeGame : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Update();
	virtual bool Render();

	bool GetDebugViewCameraInfo() { return _bViewCameraInfo; }
	void SetDebugViewCameraInfo(bool b) { _bViewCameraInfo = b; }
	bool GetUseCollision() { return _bUseCollision; }
	void SetUseCollision(bool b) { _bUseCollision = b; }


	class CameraActor* GetCamera() { return _Camera; }
	class PlayerActor* GetPlayer(int n = 0);
protected:

	class CameraActor* _Camera;
	class PlayerActor* _Player[2];

	class EffectController* _EffectController;

private:
	bool LoadStage(const std::string path,const std::string jsname);
};
