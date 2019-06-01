#include "UserInterfaceManager.h"

UserInterfaceManager* UserInterfaceManager::GetInstance() {
	static UserInterfaceManager instance;
	return &instance;
}

void UserInterfaceManager::RegisterInterface(UserInterface& newInterface) {
	interfaceRegister.push_back(&newInterface);
}
void UserInterfaceManager::Close(void) {
	// Delete all the levels.
	const size_t interfaceRegisterSize = interfaceRegister.size();
	for (size_t i = 0; i < interfaceRegisterSize; i++) {
		delete interfaceRegister[i];
	}
}
bool UserInterfaceManager::Update(const float & deltaTime) {
	return true;
}
bool UserInterfaceManager::Render(void) {
	return true;
}
