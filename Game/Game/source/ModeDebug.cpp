#include "ModeDebug.h"

bool ModeDebug::Initialize()
{
	return false;
}

bool ModeDebug::Terminate()
{
	return false;
}

bool ModeDebug::Process()
{
	return false;
}

bool ModeDebug::Render()
{
	base::Render();
	DrawFormatString(0, 0, GetColor(205, 25, 25), "Debug");
	return false;
}
