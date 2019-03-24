#include "Background.h"

Background::Background(const std::string name) {
	this->name = name;
}
Background::~Background() {
}

const std::string Background::GetName() {
	return name;
}
