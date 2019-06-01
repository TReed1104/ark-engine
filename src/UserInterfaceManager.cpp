#include "UserInterfaceManager.h"

UserInterfaceManager* UserInterfaceManager::GetInstance() {
	static UserInterfaceManager instance;
	return &instance;
}

void UserInterfaceManager::RegisterInterface(UserInterface & newInterface) {

}
bool UserInterfaceManager::Update(const float & deltaTime) {
	return true;
}
bool UserInterfaceManager::Render(void) {
	return true;
}
