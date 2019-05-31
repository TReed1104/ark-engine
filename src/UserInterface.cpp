#include "UserInterface.h"

Engine* UserInterface::Engine_Pointer;

UserInterface::UserInterface(const std::string& name, const std::string& configFile) {
}
UserInterface::~UserInterface() {
}

const std::string UserInterface::GetName(void) {
	return this->name;
}
void UserInterface::Update(const float& deltaTime) {

}
void UserInterface::Draw(void) {

}
