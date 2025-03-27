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

	// アイテム
	int _ItemList[2];
	int _ItemNum;

	// 合体
	float _MaxMargeTime;
	float _MargeTimer;
	float _MaxDashTime;
	float _DashTimer;

	// ボスのHP
	float	_BossHP;

	// ステージ選択
	int		_SelectStage;
	bool	_IsRestart;

	bool _IsGroupAttack;

	// タイマー用
	int		_StartTime;		// 開始時間
	int		_EndTime;		// 終了時間
	int		_PauseTime;		// 一時停止時間

	std::string _Text;
};

// 他のソースでgGlobalを使えるように
extern ApplicationGlobal		gGlobal;

