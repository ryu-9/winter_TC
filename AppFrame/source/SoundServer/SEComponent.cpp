#include "SEComponent.h"

SEComponent::SEComponent(ActorClass* owner,ActorClass* listner,bool deadtype)
	:SoundComponent(owner,deadtype)
	, _Listener(listner)
{
	SoundServer::GetInstance()->Create(_Owner, "fire","fire","fire");
	SetSourceVoice(SoundServer::GetInstance()->GetSourceVoice(_Owner, "fire"));
	_SV[0]->Play();

	float r = rand() % 25;
	r *= 0.01f;
	if (rand() % 2 == 0) r *= -1;
	_SV[0]->SetPitch(1.0f + r);
}

SEComponent::~SEComponent() {

}

void SEComponent::Update() {
	// 距離による音量減衰
	auto pos = _Owner->GetPosition();
	auto lpos = _Listener->GetPosition();
	//auto vec = VSub(lpos, pos);
	//auto dist = VSize(vec);

	auto dist = VSize(VSub(pos, lpos));

	// 点音源のパターン
	auto db = 10 * log10((10000)/(dist*dist));

	_SV[0]->SetVolumeDB(db);

	// パンニング
	// リスナーがプレイヤーの場合

	

	SoundComponent::Update();
}

void SEComponent::Receive(int message) {

}
