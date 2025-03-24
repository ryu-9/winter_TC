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

	void PreLoad();

	class CameraActor* GetCamera() { return _Camera; }
	class PlayerActor* GetPlayer(int n = 0);
	class UITextActor* GetUIT() { return _UIT; }

	bool debug_hcoll_flag;
	bool debug_mcoll_flag;
protected:

	class CameraActor* _Camera;
	class PlayerActor* _Player[2];

	class EffectController* _EffectController;

	class UITextActor* _UIT;

	
private:
	bool 
	LoadStage(const std::string path,const std::string jsname);
};
