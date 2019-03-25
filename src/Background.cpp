#include "Background.h"
#include "Engine.h"

Engine* Background::Engine_Pointer;

Background::Background(const std::string& name, const std::string& texturePath) {
	this->name = name;
	isLoaded = Load(texturePath);
}
Background::~Background() {
	// If the texture has been created on the heap, delete it
	if (texture != nullptr) {
		delete texture;
	}
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
	if (texturePath != "") {
		texture = new Texture(name, texturePath, true, false);

		return false;
	}
	else {
		return false;
	}
}
