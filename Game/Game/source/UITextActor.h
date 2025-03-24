#pragma once
#include "appframe.h"

class UITextComponent;

class UITextActor :  public UIChipClass{
public:
	UITextActor(class ModeBase* base);
	~UITextActor();
	void UpdateActor() override;

	void AddText(std::string jsonkey, std::string jsonkey2 = "",bool isonce = false);

private:
	std::deque<class UITextComponent*> _UIText;
	std::map<std::string,int> _Data;
};

