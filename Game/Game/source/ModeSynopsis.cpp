#include "ModeSynopsis.h"

bool ModeSynopsis::Initialize()
{
	return false;
}

bool ModeSynopsis::Terminate()
{
	return false;
}

bool ModeSynopsis::Process()
{
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// ���̃��[�h��艺�̃��C���[��Process()���Ă΂Ȃ�
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	if (trg & PAD_INPUT_2)
	{
		ModeServer::GetInstance()->Del(this);
	}
	return false;
}

bool ModeSynopsis::Render()
{
	base::Render();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Synopsis");
	return false;
}
