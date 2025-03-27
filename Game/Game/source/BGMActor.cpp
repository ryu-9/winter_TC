#include "BGMActor.h"
#include "PlayerActor.h"
#include "ModeGame.h"
#include "appframe.h"
#include "applicationGlobal.h"

BGMActor::BGMActor(ModeBase* mode) 
	: ActorClass(mode)
	, _SetFilter(false)
	, _PlayBGM(0)
{
	SS::GetInstance()->Create(this, "bgm1", "BGM", "bgm1",0,true);
	
	SS::GetInstance()->GetSourceVoice(this, "bgm1")->SetVolume(0.4);
	if (gGlobal._SelectStage != 3) {
		SS::GetInstance()->Create(this, "bgm2", "BGM", "bgm2", 44100 * 3);
		SS::GetInstance()->GetSourceVoice(this, "bgm2")->SetVolume(0.4);
		SS::GetInstance()->Create(this, "bgm3", "BGM", "bgm3",0,true);
		SS::GetInstance()->GetSourceVoice(this, "bgm3")->SetVolume(0.4);
	}
	
	//TODO: �W�c���BGM
}

BGMActor::~BGMActor() {
	SS::GetInstance()->DeleteSourceVoice(this, "bgm1");
	
	if (gGlobal._SelectStage != 3) {
		SS::GetInstance()->GetSourceVoice(this, "bgm2")->Stop();
		SS::GetInstance()->DeleteSourceVoice(this, "bgm2");
		SS::GetInstance()->GetSourceVoice(this, "bgm3")->Stop();
		SS::GetInstance()->DeleteSourceVoice(this, "bgm3");
	}
}

void BGMActor::UpdateActor() {
	auto bgm1 = SS::GetInstance()->GetSourceVoice(this, "bgm1");
	auto bgm2 = SS::GetInstance()->GetSourceVoice(this, "bgm2");
	auto bgm3 = SS::GetInstance()->GetSourceVoice(this, "bgm3");
	if (_PlayBGM == 0) {
		XAUDIO2_FILTER_PARAMETERS param;
		param.Type = LowPassOnePoleFilter;
		param.Frequency = 1.f;
		param.OneOverQ = 1.0;
		bgm1->SetFilter(param);
		bgm1->Play();

		_PlayBGM = 1;
	}
	auto n = dynamic_cast<ModeGame*>(_Mode)->GetPlayer()->GetModeNum();
	if (n >= 2) { n = 1; }

	if (_PlayBGM == 1) {
		if (n >= 1 && bgm2 != nullptr) {
			bgm1->Stop();
			bgm2->Play();
			_PlayBGM = 2;
		}
		if (gGlobal._IsGroupAttack) {
			bgm1->Stop();
			bgm3->Play();
			_PlayBGM = 3;
		}
		auto n2 = dynamic_cast<ModeGame*>(_Mode)->GetPlayer(1)->GetModeNum();
		if (n == -1 || n2 == -1) {
			if (_SetFilter == false) {
				auto f = new SVItemFilterFade(bgm1);
				f->SetFadeTime(600);
				f->SetFilParam(0.1);
				_SetFilter = true;
			}
		}
		else if (n == 0) {
			if (_SetFilter == true) {
				auto f =new SVItemFilterFade(bgm1);
				f->SetFadeTime(600);
				f->SetFilParam(1);
				_SetFilter = false;
			}
		}
	}
	if (_PlayBGM ==2) {
		if (n < 1) {
			bgm2->Stop();
			SoundServer::GetInstance()->DeleteSourceVoice(this, "bgm2");
			auto b = SS::GetInstance()->Create(this, "bgm2", "BGM", "bgm2", 44100 * 3);
			b->SetVolume(0.4);
			if (gGlobal._IsGroupAttack) {
				bgm3->SetVolume(0.4);
				bgm3->Play();
				_PlayBGM = 3;
			} else {
				bgm1->SetVolume(0.4);
				bgm1->Play();
				_PlayBGM = 1;
			}
		}

	}
	if (_PlayBGM == 3) {
		if (n >= 1 && bgm2 != nullptr) {
			bgm3->Stop();
			SoundServer::GetInstance()->DeleteSourceVoice(this, "bgm3");
			auto b = SS::GetInstance()->Create(this, "bgm3", "BGM", "bgm3", 0, true);
			b->SetVolume(0.4);
			bgm2->SetVolume(0.4);
			SS::GetInstance()->GetSourceVoice(this, "bgm2")->Play();
			_PlayBGM = 2;
		}
		if (!gGlobal._IsGroupAttack) {
			bgm3->Stop();
			SoundServer::GetInstance()->DeleteSourceVoice(this, "bgm3");
			auto b = SS::GetInstance()->Create(this, "bgm3", "BGM", "bgm3", 0, true);
			b->SetVolume(0.4);
			bgm1->SetVolume(0.4);
			SS::GetInstance()->GetSourceVoice(this, "bgm1")->Play();
			_PlayBGM = 1;
		}
	}
	SS::GetInstance()->Update(this);
}
