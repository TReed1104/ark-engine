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

	exportData["position"] = std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z);
	exportData["draw_position"] = std::to_string(drawPosition.x) + ", " + std::to_string(drawPosition.y) + ", " + std::to_string(drawPosition.z);
	exportData["grid_position"] = std::to_string(gridPosition.x) + ", " + std::to_string(gridPosition.y);
	if (indexOfShader != -1) {
		exportData["shader"] = Engine_Pointer->shaderRegister[indexOfShader]->GetName();
	}
	else {
		exportData["shader"] = "Not Set";
	}
	exportData["texture"] = texture->GetName();
	exportData["model"] = model.GetName();
	if (animationIndex != -1) {
		exportData["current_animation"] = animations[animationIndex].GetName();
	}
	else {
		exportData["current_animation"] = "Not Set";
	}

	return exportData;
}
void Item::LoadAnimations(void) {
}
void Item::AnimationStateHandler(void) {
}
void Item::AnimationIndexHandler(void) {
}