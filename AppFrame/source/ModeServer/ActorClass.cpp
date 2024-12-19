#include "ActorClass.h"
#include "ModeBase.h"

ActorClass::ActorClass(ModeBase* mode)
	:_State(State::eActive)
	, _Position(VGet(0, 0, 0))
	, _Direction(VGet(0, 0, -1))
	, _Rotation(VGet(1, 0, 0))
	, _Rotation2(VGet(0, 1, 0))
	, _Size(VGet(0.5, 0.5, 0.5))
	, _Mode(mode)
{
	_Mode->AddActor(this);
}

ActorClass::~ActorClass()
{
	// ���
	_Mode->RemoveActor(this);
	int tmp = _Components.size();
	for (int i = 0, j = 0; i + j < tmp; i++) {
		auto itr = _Components[i];
		delete itr;
		i--;
		j++;
	}
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
}



void ActorClass::AddComponent(Component* component) { // �R���|�[�l���g�ǉ�
	int Order = component->GetUpdateOrder();
	auto comp = _Components.begin();
	for (; comp != _Components.end(); ++comp) { // �}������ʒu������
		if (Order < (*comp)->GetUpdateOrder()) {
			break;
		}
	}

	_Components.insert(comp, component); // �R���|�[�l���g�ǉ�
}

void ActorClass::RemoveComponent(Component* component) {
	auto comp = std::find(_Components.begin(), _Components.end(), component);
	if (comp != _Components.end()) {
		_Components.erase(comp);
	}
}


template <typename T>
Component* ActorClass::GetComponent(T type)
{
	for (auto comp : _Components) {
		T comp = dynamic_cast<T>(comp);
		if (comp != nullptr) {
			return comp;
		}
	}
	return nullptr;
}



void ActorClass::Send(int message)
{
	auto comp = _Components.size();
	for (auto i = 0; i < comp;i++) {
		_Components[i]->Receive(message);
	}
}

