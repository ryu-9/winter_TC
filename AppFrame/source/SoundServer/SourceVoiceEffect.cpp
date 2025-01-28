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
	// モードから時間取得
	// 仮
	_Cnt += 16;

	// イージングで音量を変更
	auto tmvl = (_End - _Start) * ((float)_Cnt / (float)_Tm) + _Start;
	_SV->SetVolume(tmvl);
}

SV3DPan::SV3DPan()
{
}

void SV3DPan::Set(float pan, int tm)
{
}

void SV3DPan::Update()
{
}

