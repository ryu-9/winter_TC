#pragma once
#include "appframe.h"

#include "CameraActor.h"

// 計算用マクロ
#define	PI	(3.1415926535897932386f)
#define	DEG2RAD(x)			( ((x) / 180.0f ) * PI )
#define	RAD2DEG(x)			( ((x) * 180.0f ) / PI )

class ModeStage :public ModeBase {
	typedef ModeBase base;

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Update();
	virtual bool Render();

	class CameraActor* GetCamera() { return _Camera; }
protected:
	bool LoadStage(const std::string path, const std::string jsname);
	class CameraActor* _Camera;
	int _StageNum;
	std::vector<std::string> _StageVec;

	void StageClear();
};

