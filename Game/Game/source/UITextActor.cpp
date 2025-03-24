#include "UITextActor.h"
#include "UITextComponent.h"

UITextActor::UITextActor(ModeBase* base) 
	:UIChipClass(base,VGet(0,0,0), "res/UI/UI_MESSAGE2.png",190)
{

}

UITextActor::~UITextActor() {
}

void UITextActor::UpdateActor() {
	if (_UIText.size() == 0) { return; }
	if (_UIText.front()->GetState() == "end") {
		delete _UIText.front();
		_UIText.pop_front();
	}
	if (_UIText.front()->GetState() == "wait") {
		if (_UIText.size() > 1) {
			delete _UIText.front();
			_UIText.pop_front();
		}
	}
	if (_UIText.front()->GetActive() == false) {
		_UIText.front()->SetActive(true);
	}
}

void UITextActor::AddText(std::string jsonkey, std::string jsonkey2,bool isonce) {
	if (isonce == true) {
		if (_Data.count(jsonkey2) >= 1) { return; }
	}
	auto t = new UITextComponent(this, jsonkey, jsonkey2);
	_UIText.push_back(t);
	_Data[jsonkey2] += 1;
}
