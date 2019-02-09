#include "Model.h"
#include "Engine.h"

// Model definitions
Engine* Model::Engine_Pointer;

Model::Model(const std::string& modelPath, const bool& load) {
	this->name = modelPath;
	if (modelPath != "" && load) {
		Assimp::Importer importer;	// An importer for importing the model data.
		const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_GenNormals);		// Read the Model file.

		// Loop through each mesh in the loaded model.
		for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
			Model::Mesh currentMesh = Model::Mesh();

			for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
				// Get the vertices from assimp.
				currentMesh.vertexPositions.push_back(glm::vec3(scene->mMeshes[i]->mVertices[j].x, scene->mMeshes[i]->mVertices[j].y, scene->mMeshes[i]->mVertices[j].z));

				// If the mesh is setup for texturing, get all the textures from assimp.
				if (scene->mMeshes[i]->mTextureCoords[0] != NULL) {
					currentMesh.isSetupForTextures = true;
					currentMesh.uvs.push_back(glm::vec2(scene->mMeshes[i]->mTextureCoords[0][j].x, scene->mMeshes[i]->mTextureCoords[0][j].y));
				}

				// Populate the colour buffer with a default value.
				currentMesh.colourData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

				// Check if the current mesh has surface normals setup ready for lighting.
				if (scene->mMeshes[i]->mNormals != NULL) {
					currentMesh.surfaceNormals.push_back(glm::vec3(scene->mMeshes[i]->mNormals[j].x, scene->mMeshes[i]->mNormals[j].y, scene->mMeshes[i]->mNormals[j].z));
				}
			}
			// Loops through the array of indices, pushing each of them to the index vector individually, this is because they are used individually whilst drawing instead of in 3s.
			for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
				currentMesh.indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[0]);
				currentMesh.indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[1]);
				currentMesh.indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[2]);
			}
			currentMesh.BindBuffers();
			meshes.push_back(currentMesh);
		}
		Engine_Pointer->engineDebugger.WriteLine(">>>> Model loaded! - " + name);
		importer.FreeScene();						// Cleans up the loader.
	}
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