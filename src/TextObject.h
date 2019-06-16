#ifndef ARKENGINE_TEXTOBJECT_H_
#define ARKENGINE_TEXTOBJECT_H_

#include <string>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"
#include "Font.h"
#include "DataBinding.h"

class Engine;

class TextObject {
public:
	static Engine* Engine_Pointer;

	TextObject(const std::string& name, const std::string& text, Font* font, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& colour = glm::vec3(1.0f), const bool& isEnabled = false, const bool& useCamera = false);
	~TextObject();

	void Update(const float& deltaTime);
	void Draw(void);

	const std::string GetName(void);
	const bool IsEnabled(void);
	void SetEnabledState(const bool& enableState);
	const bool HasDataBindings(void);
	void UpdateText(const std::string& newText);
	void UpdateFont(Font* font);
	const glm::vec3 GetPosition(void);
	void Reposition(const glm::vec3& newPosition);

	// Data binding
	void RegisterDataBinding(DataBinding* newDataBinding);
	void RemoveDataBinding(const std::string& bindingID);
	void ExecuteDataBindings(void);

private:
	// General
	std::string name;
	bool isEnabled = false;
	bool hasDataBindings = false;
	std::string baseText = "";
	std::string textToRender = "";		// String the class will represent
	Font* font;
	std::vector<DataBinding*> dataBindingRegister;
	
	// Rendering
	bool useCamera;
	int indexOfShader;
	glm::vec3 colour;
	std::vector<Glyph> glyphs;	// The glyphs making up the text
	Model model;
	glm::vec3 position;
	float rotation;
	glm::vec3 scale;

	void LoadText();
	void UpdatePosition(void);
	void UpdateRotation(void);
	void UpdateScale(void);
};

#endif