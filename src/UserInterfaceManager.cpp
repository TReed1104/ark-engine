#include "UserInterfaceManager.h"

UserInterfaceManager* UserInterfaceManager::GetInstance() {
	static UserInterfaceManager instance;
	return &instance;
}

void UserInterfaceManager::RegisterInterface(UserInterface& newInterface) {
	interfaceRegister.push_back(&newInterface);
}
void UserInterfaceManager::DeleteInterface(const std::string& interfaceName) {
	// Delete the specified level.
	const size_t interfaceRegisterSize = interfaceRegister.size();
	for (size_t i = 0; i < interfaceRegisterSize; i++) {
		if (interfaceRegister[i]->GetName() == interfaceName) {
			// This needs double checking?
			delete interfaceRegister[i];
			interfaceRegister.erase(interfaceRegister.begin() + i);
			break;
		}
	}
}
void UserInterfaceManager::DeleteAllInterfaces(void) {
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
