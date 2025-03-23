#include "BGMActor.h"
#include "PlayerActor.h"
#include "ModeGame.h"

BGMActor::BGMActor(ModeBase* mode) 
	: ActorClass(mode)
{
	SS::GetInstance()->Create(this, "bgm1", "BGM", "bgm1",0,true);
	SS::GetInstance()->Create(this, "bgm2", "BGM", "bgm2",44100*3);
	SS::GetInstance()->GetSourceVoice(this, "bgm1")->SetVolume(0.4);
	SS::GetInstance()->GetSourceVoice(this, "bgm2")->SetVolume(0.4);
	//TODO: W’cí‚ÌBGM
}

BGMActor::~BGMActor() {
	SS::GetInstance()->GetSourceVoice(this, "bgm1")->Stop();
	SS::GetInstance()->GetSourceVoice(this, "bgm2")->Stop();
	SS::GetInstance()->DeleteSourceVoice(this, "bgm1");
	SS::GetInstance()->DeleteSourceVoice(this, "bgm2");
}

void BGMActor::UpdateActor() {
	auto bgm1 = SS::GetInstance()->GetSourceVoice(this, "bgm1");
	auto bgm2 = SS::GetInstance()->GetSourceVoice(this, "bgm2");
	if (!bgm1->IsPlay()&&!bgm2->IsPlay()) {
		bgm1->Play();
	}
	auto n = dynamic_cast<ModeGame*>(_Mode)->GetPlayer()->GetModeNum();
	if (n >= 2) { n = 1; }

	if (bgm1->IsPlay()) {
		if (n >= 1) {
			bgm1->Stop();
			bgm2->Play();
		}
		auto n2 = dynamic_cast<ModeGame*>(_Mode)->GetPlayer(1)->GetModeNum();
		if (n == -1 || n2 == -1) {
			if (_SetFilter == false) {
				XAUDIO2_FILTER_PARAMETERS param;
				param.Type = LowPassOnePoleFilter;
				param.Frequency = 0.05;
				param.OneOverQ = 1.0;
				bgm1->SetFilter(param);
				bgm2->SetFilter(param);
				_SetFilter = true;
			}
		}
		else if (n == 0) {
			if (_SetFilter == true) {
				XAUDIO2_FILTER_PARAMETERS param;
				param.Type = LowPassOnePoleFilter;
				param.Frequency = 1;
				param.OneOverQ = 1.0;
				bgm1->SetFilter(param);
				bgm2->SetFilter(param);
				_SetFilter = false;
			}
		}
	}
	if (bgm2->IsPlay()) {
		if (n < 1) {
			bgm2->Stop();
			bgm2->ResetPlayTm(44100 * 3);
			bgm1->Play();
		}
	}
	SS::GetInstance()->Update(this);
}
