#pragma once
#include "appframe.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class PlayerActor* owner, int updateOrder = 10);
	virtual ~MoveComponent();

	void ProcessInput() override;

	void Receive(int message);
private:
	class PlayerActor* _pOwner;
	float _colSubY;	// �R���W�������莞��Y�␳(���ʒu�j
	float _AddSpeed;
};

