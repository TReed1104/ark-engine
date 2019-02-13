#include "Item.h"
#include "Engine.h"

Item::Item(const std::string& filePath) : GameObject(filePath) {
	if (configFile->IsLoaded()) {
		name = configFile->Get<std::string>("item.id");
		isLoaded = true;
	}
	else {
		Engine_Pointer->engineDebugger.WriteLine(">>>> Item failed to load Config File: " + filePath);
		isLoaded = false;
	}
}
Item::~Item() {
}
void Item::LoadAnimations(void) {
}
void Item::AnimationStateHandler(void) {
}
void Item::AnimationIndexHandler(void) {
}