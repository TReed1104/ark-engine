#include "Background.h"

Background::Background(const std::string name) {
	this->name = name;
}
Background::~Background() {
}

void Background::Update(const float & deltaTime) {

}
void Background::Draw(void) {

}
const std::string Background::GetName() {
	return name;
}
