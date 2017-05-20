#include "Model.h"
#include "Engine.h"

// Model definitions
Engine* Model::Engine_Pointer;

Model::Model(const std::string& name) {
	this->name = name;
}
Model::~Model() {

}

void Model::SetMeshParents() {
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].SetParent(*this);
		meshes[i].SetTranslation();
		meshes[i].SetRotation();
		meshes[i].SetScale();
	}
}
void Model::Translate(const glm::vec3 & translation) {
	modelTranslationMatrix = glm::translate(glm::mat4(1.0f), translation);
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].SetTranslation(true);
	}
}
void Model::Rotate(const float & rotationAngle, const glm::vec3 & rotationAxis) {
	modelRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].SetRotation(true);
	}
}
void Model::Scale(const glm::vec3 & scale) {
	modelScaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].SetScale(true);
	}
}
void Model::SetMeshTranslation(const int& indexOfMesh, const glm::vec3 & translation) {
	meshes[indexOfMesh].SetTranslation(false, translation);
}
void Model::SetMeshRotation(const int &indexOfMesh, const float & rotationAngle, const glm::vec3 & rotationAxis) {
	meshes[indexOfMesh].SetRotation(false, rotationAngle, rotationAxis);
}
void Model::SetMeshScale(const int& indexOfMesh, const glm::vec3 & scale) {
	meshes[indexOfMesh].SetScale(false, scale);
}