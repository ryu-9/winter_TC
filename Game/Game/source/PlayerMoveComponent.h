#pragma once
#include "appframe.h"

class PlayerMoveComponent : public MoveComponent
{
public:
	PlayerMoveComponent(class PlayerActor* owner, int updateOrder = 10);
	virtual ~PlayerMoveComponent();

	void ProcessInput() override;

	void Receive(int message);
private:
	class PlayerActor* _pOwner;
	float _colSubY;	// �R���W�������莞��Y�␳(���ʒu�j

};

