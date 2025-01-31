#pragma once

namespace Enemy {
	struct SEARCH {
		float dist;		// ’Tõ”ÍˆÍ
		float angle;	// ’TõŠp“x
	};

	struct SEARCH_RES {
		bool isFind;
		float dist;
		class ActorClass* actor;
	};

}