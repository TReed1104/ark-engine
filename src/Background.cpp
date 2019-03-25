#include "Background.h"
#include "Engine.h"

Engine* Background::Engine_Pointer;

Background::Background(const std::string& name, const std::string& texturePath) {
	this->name = name;

}
Background::~Background() {
}

void Background::Update(const float& deltaTime) {

}
void Background::Draw(void) {

}
const std::string Background::GetName() {
	return name;
}
const bool Background::IsLoaded(void) {
	return isLoaded;
}
bool Background::Load(const std::string& texturePath) {

	return false;
}
