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
std::map<std::string, std::string> Item::ExportDataForBinding(void) {
	std::map<std::string, std::string> exportData;

	return exportData;
}
void Item::LoadAnimations(void) {
}
void Item::AnimationStateHandler(void) {
}
void Item::AnimationIndexHandler(void) {
}