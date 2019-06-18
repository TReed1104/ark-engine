#include "DataBinding.h"

DataBinding::DataBinding(const std::string& name, const BindingTargetType& targetType, const std::string& targetName, const std::string& variableName, const std::string& bindingToken) {
	this->name = name;
	this->targetType = targetType;
	this->targetName = targetName;
	this->variableName = variableName;
	this->bindingToken = bindingToken;
}
DataBinding::~DataBinding() {
}

const std::string DataBinding::GetName(void) {
	return this->name;
}
const DataBinding::BindingTargetType DataBinding::GetTargetType(void) {
	return this->targetType;
}
const std::string DataBinding::GetTargetName(void) {
	return this->targetName;
}
const std::string DataBinding::GetVariableName(void) {
	return this->variableName;
}
const std::string DataBinding::GetBindingToken(void) {
	return this->bindingToken;
}
