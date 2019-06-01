#include "UserInterfaceManager.h"

UserInterfaceManager* UserInterfaceManager::GetInstance() {
	static UserInterfaceManager instance;
	return &instance;
}
