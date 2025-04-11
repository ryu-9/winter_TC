#pragma once
#include <vector>

namespace Enemy {
	struct SEARCH {
		float dist;		// 探索範囲
		float angle;	// 探索角度
	};

	struct MOVE {
		int duration;		// 移動時間_基本
		int dur_rand;		// 移動時間_ランダム分
		int cooltime;		// クールタイム_基本
		int cool_rand;		// クールタイム_ランダム分
		float dist;			// 移動距離_基本
		int dist_rand;	// 移動距離_ランダム分
	};


}