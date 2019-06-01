#include "UserInterfaceManager.h"

UserInterfaceManager* UserInterfaceManager::GetInstance() {
	static UserInterfaceManager instance;
	return &instance;
}

bool UserInterfaceManager::Update(const float & deltaTime) {
	return true;
}
bool UserInterfaceManager::Render(void) {
	return true;
}
