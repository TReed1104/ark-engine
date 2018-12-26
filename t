[1mdiff --git a/src/GameObject.cpp b/src/GameObject.cpp[m
[1mindex 865e5f1..d68b13e 100644[m
[1m--- a/src/GameObject.cpp[m
[1m+++ b/src/GameObject.cpp[m
[36m@@ -43,10 +43,6 @@[m [mGameObject::GameObject(const std::string & scriptPath) {[m
 	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));[m
 	model.Scale(scale);[m
 [m
[31m-	doTranslation = false;[m
[31m-	doRotation = false;[m
[31m-	doScalar = false;[m
[31m-[m
 	sourceFrameSize = Engine_Pointer->tileTextureFrameSize;[m
 }[m
 GameObject::~GameObject(void) {[m
[36m@@ -57,10 +53,6 @@[m [mGameObject::~GameObject(void) {[m
 [m
 // Game Run Time.[m
 void GameObject::Update(const float& deltaTime) {[m
[31m-	// Check if the object's velocity has been set, if so then allow the transformation matrix to be updated.[m
[31m-	if ((velocity != glm::vec2(0.0f, 0.0f) || velocitySnap != glm::vec2(0.0f, 0.0f))) {[m
[31m-		doTranslation = true;[m
[31m-	}[m
 [m
 	// Apply transformations[m
 	UpdatePosition();[m
[36m@@ -132,27 +124,18 @@[m [mvoid GameObject::Reposition(const glm::vec2& newPosition) {[m
 	model.Translate(drawPosition);[m
 }[m
 void GameObject::UpdatePosition() {[m
[31m-	if (doTranslation) {[m
[31m-		position += glm::vec3(velocity, 0.0f);[m
[31m-		position += glm::vec3(velocitySnap, 0.0f);[m
[31m-		gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(position.x, position.y));[m
[31m-		drawPosition = (position + glm::vec3(drawOffset, 0.0f));[m
[31m-		boundingBox.UpdatePosition(glm::vec2(position.x, position.y) + boundingBoxOffset);[m
[31m-		model.Translate(drawPosition);[m
[31m-		doTranslation = false;[m
[31m-	}[m
[32m+[m	[32mposition += glm::vec3(velocity, 0.0f);[m
[32m+[m	[32mposition += glm::vec3(velocitySnap, 0.0f);[m
[32m+[m	[32mgridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(position.x, position.y));[m
[32m+[m	[32mdrawPosition = (position + glm::vec3(drawOffset, 0.0f));[m
[32m+[m	[32mboundingBox.UpdatePosition(glm::vec2(position.x, position.y) + boundingBoxOffset);[m
[32m+[m	[32mmodel.Translate(drawPosition);[m
 }[m
 void GameObject::UpdateRotation() {[m
[31m-	if (doRotation) {[m
[31m-		model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));[m
[31m-		doRotation = false;[m
[31m-	}[m
[32m+[m	[32mmodel.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));[m
 }[m
 void GameObject::UpdateScale() {[m
[31m-	if (doScalar) {[m
[31m-		model.Scale(scale);[m
[31m-		doScalar = false;[m
[31m-	}[m
[32m+[m	[32mmodel.Scale(scale);[m
 }[m
 void GameObject::LoadAnimations() {[m
 	animations.clear();[m
[1mdiff --git a/src/GameObject.h b/src/GameObject.h[m
[1mindex 42d9ca7..b151022 100644[m
[1m--- a/src/GameObject.h[m
[1m+++ b/src/GameObject.h[m
[36m@@ -53,9 +53,6 @@[m [mpublic:[m
 	// Transformations[m
 	float rotation;[m
 	glm::vec3 scale;[m
[31m-	bool doTranslation;		// Flag controlling if the Object needs translating[m
[31m-	bool doRotation;		// Flag controlling if the Object needs rotating[m
[31m-	bool doScalar;			// Flag controlling if the Object needs scaling[m
 	[m
 	GameObject(const std::string& scriptPath = "NO SCRIPT");[m
 	~GameObject(void);[m
[1mdiff --git a/src/RenderableText.cpp b/src/RenderableText.cpp[m
[1mindex a2c04e6..1d948aa 100644[m
[1m--- a/src/RenderableText.cpp[m
[1m+++ b/src/RenderableText.cpp[m
[36m@@ -4,21 +4,22 @@[m
 Engine* RenderableText::Engine_Pointer;[m
 [m
 RenderableText::RenderableText(const std::string& name, const std::string& text, Font* font, const glm::vec3& position, const glm::vec3& colour, const bool& useCamera) {[m
[32m+[m	[32m// Informational[m
 	this->name = name;[m
 	this->text = text;[m
[31m-	this->font = font;[m
[31m-	this->colour = colour;[m
[31m-	this->model = Model(text, false);[m
[31m-	this->indexOfTextShader = Engine_Pointer->GetIndexOfShader("renderable Text");[m
[31m-	this->doTranslation = false;[m
[31m-	this->doRotation = false;[m
[31m-	this->doScalar = false;[m
[32m+[m
[32m+[m	[32m// Transforms[m
 	this->position = position;[m
 	this->rotation = 0.0f;[m
 	this->scale = glm::vec3(1.0f);[m
[32m+[m[41m	[m
[32m+[m	[32m// Rendering[m
[32m+[m	[32mthis->colour = colour;[m
[32m+[m	[32mthis->indexOfTextShader = Engine_Pointer->GetIndexOfShader("renderable Text");[m
[32m+[m	[32mthis->model = Model(text, false);[m
 	this->useCamera = useCamera;[m
 [m
[31m-	LoadText();[m
[32m+[m	[32mLoadText(font);[m
 }[m
 RenderableText::~RenderableText() {[m
 [m
[36m@@ -76,7 +77,8 @@[m [mvoid RenderableText::Enable(void) {[m
 void RenderableText::Disable(void) {[m
 [m
 }[m
[31m-void RenderableText::LoadText(void) {[m
[32m+[m[32mvoid RenderableText::LoadText(Font* font) {[m
[32m+[m	[32mglyphs.clear();[m
 	const size_t lengthOfText = text.size();[m
 	for (size_t i = 0; i < lengthOfText; i++) {[m
 		Glyph& currentGlyph = font->GetGlyph(text[i]);[m
[36m@@ -101,20 +103,11 @@[m [mvoid RenderableText::LoadText(void) {[m
 	}[m
 }[m
 void RenderableText::UpdatePosition() {[m
[31m-	if (doTranslation) {[m
[31m-		model.Translate(position);[m
[31m-		doTranslation = false;[m
[31m-	}[m
[32m+[m	[32mmodel.Translate(position);[m
 }[m
 void RenderableText::UpdateRotation() {[m
[31m-	if (doRotation) {[m
[31m-		model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));[m
[31m-		doRotation = false;[m
[31m-	}[m
[32m+[m	[32mmodel.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));[m
 }[m
 void RenderableText::UpdateScale() {[m
[31m-	if (doScalar) {[m
[31m-		model.Scale(scale);[m
[31m-		doScalar = false;[m
[31m-	}[m
[31m-}[m
\ No newline at end of file[m
[32m+[m	[32mmodel.Scale(scale);[m
[32m+[m[32m}[m
[1mdiff --git a/src/RenderableText.h b/src/RenderableText.h[m
[1mindex 1b59e29..95c4065 100644[m
[1m--- a/src/RenderableText.h[m
[1m+++ b/src/RenderableText.h[m
[36m@@ -19,7 +19,6 @@[m [mpublic:[m
 [m
 	std::string name;			// id of the text[m
 	std::string text;			// String the class will represent[m
[31m-	Font* font;[m
 	glm::vec3 colour;[m
 	std::vector<Glyph> glyphs;	// The glyphs making up the text[m
 	Model model;[m
[36m@@ -31,12 +30,9 @@[m [mpublic:[m
 	glm::vec3 position;[m
 	bool useCamera;[m
 [m
[31m-	// Transformations and flags.[m
[32m+[m	[32m// Transformations[m
 	float rotation;[m
 	glm::vec3 scale;[m
[31m-	bool doTranslation;[m
[31m-	bool doRotation;[m
[31m-	bool doScalar;[m
 [m
 	RenderableText(const std::string& name, const std::string& text, Font* font, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& colour = glm::vec3(1.0f), const bool& useCamera = false);[m
 	~RenderableText();[m
[36m@@ -48,7 +44,7 @@[m [mpublic:[m
 	void Disable(void);[m
 [m
 private:[m
[31m-	void LoadText(void);[m
[32m+[m	[32mvoid LoadText(Font* font);[m
 	void UpdatePosition(void);[m
 	void UpdateRotation(void);[m
 	void UpdateScale(void);[m
