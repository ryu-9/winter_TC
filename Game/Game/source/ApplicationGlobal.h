#include "appframe.h"

class ApplicationGlobal
{
public:
	ApplicationGlobal();
	virtual ~ApplicationGlobal();

	bool Init();

public:

	// ステージ解放データ
	int	_Stageflg;
};

// 他のソースでgGlobalを使えるように
extern ApplicationGlobal		gGlobal;

