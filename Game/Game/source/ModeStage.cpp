#include "ModeStage.h"
#include "nlohmann/json.hpp"

bool ModeStage::Initialize() {
	if (!base::Initialize()) { return false; }


	{
		std::ifstream file(path + jsonFile);
		nlohmann::json json;
		file >> json;


		nlohmann::json stage = json.at("Stage");
		for (auto& data : stage) {
			std::string name = data.at("objectName");
			auto pos = VGet(data.at("translate").at("x"), data.at("translate").at("z"), data.at("translate").at("y"));
			pos.z *= -1.f;
			auto rot = VGet(data.at("rotate").at("x"), data.at("rotate").at("z"), data.at("rotate").at("y"));
			rot.x = DEG2RAD(pos.x);
			rot.y = DEG2RAD(pos.y);
			rot.z = DEG2RAD(pos.z);
			auto scale = VGet(data.at("scale").at("x"), data.at("scale").at("z"), data.at("scale").at("y"));

			auto ac = new ActorClass(this);
			new ModelComponent(path + name + ".mv1");
			
		}
	}

	return true;
}

bool ModeStage::Terminate()
{
	return false;
}

bool ModeStage::Process()
{
	return false;
}

bool ModeStage::Update()
{
	return false;
}

bool ModeStage::Render()
{
	return false;
}
