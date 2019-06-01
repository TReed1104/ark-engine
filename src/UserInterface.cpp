#include "UserInterface.h"
#include "Engine.h"

Engine* UserInterface::Engine_Pointer;

UserInterface::UserInterface(const std::string& filePath) {
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
