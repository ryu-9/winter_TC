#pragma once
#include "dxlib.h"


class FpsController 
{
	public:
	FpsController();
	virtual ~FpsController();

	static FpsController* _lpInstance;
	static FpsController* GetInstance() { return (FpsController*)_lpInstance; }

	void Update();

	int GetDeltaTime() const { return _DeltaTime; }

private:
	int _DeltaTime;
	int _OldTime;
};

