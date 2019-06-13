#include "Model.h"
#include "Engine.h"

// Static variables and functions
Engine* Model::Engine_Pointer;
Model::Mesh Model::GenerateMesh(const glm::vec2& dimensions) {
	// Create the mesh we are going to return to the function call
	Model::Mesh mesh = Model::Mesh();

	// Specify four corner positions of the backgrounds quad 
	mesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	mesh.vertexPositions.push_back(glm::vec3(0.0f + dimensions.x, 0.0f, 0.0f));
	mesh.vertexPositions.push_back(glm::vec3(0.0f + dimensions.x, 0.0f + dimensions.y, 0.0f));
	mesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f + dimensions.y, 0.0f));

	// Set the UVs for the background quad
	mesh.uvs.push_back(glm::vec2(0.0f, 0.0f));
	mesh.uvs.push_back(glm::vec2(1.0f, 0.0f));
	mesh.uvs.push_back(glm::vec2(1.0f, 1.0f));
	mesh.uvs.push_back(glm::vec2(0.0f, 1.0f));
	mesh.isSetupForTextures = true;	// Confirm the mesh can be textured

	// Set the colour data and surface normal for each vertexPosition (these are all default)
	const size_t numberOfVertices = mesh.vertexPositions.size();
	for (size_t i = 0; i < numberOfVertices; i++) {
		mesh.colourData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));		// All white
		mesh.surfaceNormals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));	// All orientated towards our 2D cameras
	}

	// Set the indices for the mesh (the order to draw between the vertex positions
	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	mesh.indices.push_back(0);
	mesh.indices.push_back(2);
	mesh.indices.push_back(3);

	// Bind the data to buffers ready to pass to the shaders
	mesh.BindBuffers();

	// Return our generated mesh
	return mesh;
}

Model::Model(const std::string& modelPath, const bool& load) {
	name = modelPath;
	if (modelPath != "" && load) {
		this->name = StringUtilities::Split(modelPath, '\\').back();
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
		isLoaded = true;
	}
}
Model::~Model() {

}


const bool Model::IsLoaded(void) {
	return isLoaded;
}
const void Model::OverrideLoadState(const bool& newState) {
	this->isLoaded = newState;
}
void Model::SetMeshParents() {
	const size_t numberOfMeshes = meshes.size();
	for (size_t i = 0; i < numberOfMeshes; i++) {
		meshes[i].SetParent(*this);
		meshes[i].SetTranslation();
		meshes[i].SetRotation();
		meshes[i].SetScale();
	}
}
void Model::Translate(const glm::vec3& translation) {
	modelTranslationMatrix = glm::translate(glm::mat4(1.0f), translation);
	const size_t numberOfMeshes = meshes.size();
	for (size_t i = 0; i < numberOfMeshes; i++) {
		meshes[i].SetTranslation(true);
	}
}
void Model::Rotate(const float& rotationAngle, const glm::vec3& rotationAxis) {
	modelRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);
	const size_t numberOfMeshes = meshes.size();
	for (size_t i = 0; i < numberOfMeshes; i++) {
		meshes[i].SetRotation(true);
	}
}
void Model::Scale(const glm::vec3& scale) {
	modelScaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	const size_t numberOfMeshes = meshes.size();
	for (size_t i = 0; i < numberOfMeshes; i++) {
		meshes[i].SetScale(true);
	}
}
void Model::SetMeshTranslation(const int& indexOfMesh, const glm::vec3& translation) {
	meshes[indexOfMesh].SetTranslation(false, translation);
}
void Model::SetMeshRotation(const int &indexOfMesh, const float& rotationAngle, const glm::vec3& rotationAxis) {
	meshes[indexOfMesh].SetRotation(false, rotationAngle, rotationAxis);
}
void Model::SetMeshScale(const int& indexOfMesh, const glm::vec3& scale) {
	meshes[indexOfMesh].SetScale(false, scale);
}