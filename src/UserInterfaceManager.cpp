#include "UserInterfaceManager.h"

UserInterfaceManager* UserInterfaceManager::GetInstance() {
	static UserInterfaceManager instance;
	return &instance;
}

void UserInterfaceManager::RegisterInterface(UserInterface* newInterface) {
	interfaceRegister.push_back(newInterface);
}
void UserInterfaceManager::Close(void) {
	// Delete all the levels.
	const size_t interfaceRegisterSize = interfaceRegister.size();
	for (size_t i = 0; i < interfaceRegisterSize; i++) {
		delete interfaceRegister[i];
	}
}
void UserInterfaceManager::Update(const float& deltaTime) {
	// Update the interfaces
	size_t numberOfInterfaces = interfaceRegister.size();
	for (size_t i = 0; i < numberOfInterfaces; i++) {
		interfaceRegister[i]->Update(deltaTime);
	}
}
void UserInterfaceManager::Render(void) {

}
