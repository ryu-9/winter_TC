#include "SourceVoiceEffect.h"

SVFade::SVFade()
	:_Start(0.0f), _End(0.0f), _Tm(0), _Cnt(0)
{

}

SVFade::SVFade(float vol, int tm)
	:_End(vol), _Tm(tm), _Cnt(0)
{
	_Start = 0.0f;
}

void SVFade::Update()
{
}

void SVFade::SetFunc(float vol, int tm)
{
}

SVEcho::SVEcho()
{
}

void SVEcho::Update()
{
}

void SVEcho::SetFunc(float palam)
{
}
