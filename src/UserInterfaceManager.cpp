#include "UserInterfaceManager.h"

UserInterfaceManager* UserInterfaceManager::GetInstance() {
	static UserInterfaceManager instance;
	return &instance;
}

void UserInterfaceManager::RegisterInterface(UserInterface* newInterface) {
	interfaceRegister.push_back(newInterface);
}
UserInterface* UserInterfaceManager::GetUserInterface(const std::string& userInterfaceName) {
	int indexOfInterface = -1;

	// Search for the interface using the supplied ID
	const size_t numberOfInterfaces = interfaceRegister.size();
	for (size_t i = 0; i < numberOfInterfaces; i++) {
		if (interfaceRegister[i]->GetName() == userInterfaceName) {
			indexOfInterface = (int)i;
			break;
		}
	}

	// Check we found the interface
	if (indexOfInterface == -1) {
		return nullptr;
	}

	// Return the interface
	return interfaceRegister[indexOfInterface];
}
bool UserInterfaceManager::SetInterfaceActiveState(const std::string& userInterfaceName, const bool& newState) {
	UserInterface* desiredInterface = GetUserInterface(userInterfaceName);
	if (desiredInterface == nullptr) {
		return false;
	}

	desiredInterface->SetActiveState(newState);
	return true;
}
void UserInterfaceManager::Close(void) {
	// Delete all the interfaces
	for (UserInterface* userInterface : interfaceRegister) {
		delete userInterface;
		userInterface = nullptr;
	}
}
void UserInterfaceManager::Update(const float& deltaTime) {
	// Update the interfaces
	for (UserInterface* userInterface : interfaceRegister) {
		if (userInterface->IsActive()) {
			userInterface->Update(deltaTime);
		}
	}
}
void UserInterfaceManager::Render(void) {
	// Render the interfaces
	for (UserInterface* userInterface : interfaceRegister) {
		if (userInterface->IsActive()) {
			userInterface->Draw();
		}
	}
}
