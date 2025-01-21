#include "SourceVoiceEffect.h"
#include "SourceVoiceItem.h"

SVFade::SVFade()
	:_Start(0.0f), _End(0.0f), _Tm(0), _Cnt(0)
{

}


void SVFade::Set(float vol, int tm) {
	_Start = _SV->GetVolume();
	_End = vol;
	_Tm = tm;
	_Cnt = 0;
}

void SVFade::Update() {

}


SVEcho::SVEcho()
	: palam(0.0f)
{

}

void SVEcho::Set(float palam)
{
}

void SVEcho::Update()
{
}

