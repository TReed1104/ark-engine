#ifndef ARKENGINE_DATABINDING_H_
#define ARKENGINE_DATABINDING_H_

#include <string>

class DataBinding {
public:
	enum BindingTargetType { ENGINE, CAMERA, LEVEL, TILE, ITEM, ENTITY };
	DataBinding(const std::string& name, const BindingTargetType& targetType, const std::string& targetName, const std::string& bindingToken);
	~DataBinding();

	const std::string GetName(void);
	const BindingTargetType GetTargetType(void);
	const std::string GetTargetName(void);
	const std::string GetVariableName(void);
	const std::string GetBindingToken(void);

private:
	// Binding settings
	std::string name = "";
	BindingTargetType targetType = BindingTargetType::ENGINE;
	std::string targetName = "";
	std::string variableName = "";
	std::string bindingToken = "";
};

#endif