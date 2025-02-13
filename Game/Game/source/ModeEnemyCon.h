#pragma once
#include "appframe.h"
class ModeEnemyCon : public ModeBase {
	typedef ModeBase base;
public:
	ModeEnemyCon();
	~ModeEnemyCon();
	bool Initialize() override;
	bool Terminate() override;
	bool Process() override;
	bool Render() override;
private:
	
};

