#include "ModeScenario.h"

bool ModeScenario::Initialize() {
	if (!base::Initialize()) { return false; }

	return false;
}

bool ModeScenario::Terminate() {
	return false;
}

bool ModeScenario::Process() {
	return false;
}

bool ModeScenario::Render() {
	return false;
}
