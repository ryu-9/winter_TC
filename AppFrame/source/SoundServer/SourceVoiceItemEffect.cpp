#include "SourceVoiceItemEffect.h"
#include "SourceVoiceItem.h"

SourceVoiceItemEffect::SourceVoiceItemEffect(SourceVoiceItem* sv) {
	_SV = sv;
}

SourceVoiceItemEffectFade::SourceVoiceItemEffectFade(SourceVoiceItem* sv)
	:SourceVoiceItemEffect(sv) 
	, _FadeTime(0)
	, _FadeTimeMax(0)
{
	_VolumeStart = _SV->GetVolume();
	_ToDestroy = true;
}

SourceVoiceItemEffectFade::~SourceVoiceItemEffectFade() {
}

void SourceVoiceItemEffectFade::Update() {
	// TODO: Mode‚ÉG‚Á‚ÄŽžŠÔ‚ðŽæ“¾‚·‚é
	_FadeTime += 0.016;
	float rate = _FadeTime / _FadeTimeMax;
	auto vol = _VolumeStart + (_VolumeEnd - _VolumeStart) * rate;
	vol *= vol;
	_SV->SetVolume(vol);

	
}
