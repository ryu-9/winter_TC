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

	// プレイヤーのHP(Size)
	float	_PlayerHP[2] = {0.1,0.1};
	bool	_IsPlayerDead[2] ;

	// ステージ選択
	int		_SelectStage;
};

// 他のソースでgGlobalを使えるように
extern ApplicationGlobal		gGlobal;

