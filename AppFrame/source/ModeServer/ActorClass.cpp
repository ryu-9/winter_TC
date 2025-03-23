#include "ActorClass.h"
#include "ModeBase.h"
#include "../SoundServer/SoundServer.h"


ActorClass::ActorClass(ModeBase* mode)
	:_State(State::eActive)
	, _Position(VGet(0, 0, 0))
	, _Direction(VGet(0, 0, 0))
	, _Size(VGet(0.5, 0.5, 0.5))
	, _Mode(mode)
{
	_Mode->AddActor(this);
}

ActorClass::~ActorClass()
{
	// 解放
	_Mode->RemoveActor(this);
	int tmp = _Components.size();
	for (int i = 0, j = 0; i + j < tmp; i++) {
		auto itr = _Components[i];
		delete itr;
		i--;
		j++;
	}
	SoundServer::GetInstance()->Release(this);
}

void ActorClass::ProcessInput() {
	if (_State == State::eActive)
	{

		for (auto comp : _Components)
		{
			comp->ProcessInput();
		}

//		ActorInput();
	}
}

void ActorClass::Update() {
	if (_State == State::eActive || _State == State::ePreparation) {
		if (this == nullptr) {
			delete this;
			return;
		}
		if (_Components.size() > 500) {
			int test = 0;
		}
		UpdateComponents();
		UpdateActor();

	}
}

void ActorClass::UpdateComponents() {
	for (auto comp : _Components) {
		comp->Update();
	}
}


void ActorClass::UpdateActor() {
	SoundServer::GetInstance()->Update(this);
}





void ActorClass::AddComponent(Component* component) { // コンポーネント追加
	int Order = component->GetUpdateOrder();
	auto comp = _Components.begin();
	for (; comp != _Components.end(); ++comp) { // 挿入する位置を検索
		if (Order < (*comp)->GetUpdateOrder()) {
			break;
		}
	}

	_Components.insert(comp, component); // コンポーネント追加
}

void ActorClass::RemoveComponent(Component* component) {
	auto comp = std::find(_Components.begin(), _Components.end(), component);
	if (comp != _Components.end()) {
		_Components.erase(comp);
	}
}




void ActorClass::Send(int message)
{
	auto comp = _Components.size();
	for (auto i = 0; i < comp;i++) {
		_Components[i]->Receive(message);
	}
}

