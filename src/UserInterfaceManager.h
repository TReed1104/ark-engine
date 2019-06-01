#ifndef ARKENGINE_USERINTERFACE_MANAGER_H_
#define ARKENGINE_USERINTERFACE_MANAGER_H_

class UserInterfaceManager {
public:
	static UserInterfaceManager* GetInstance();

	UserInterfaceManager(UserInterfaceManager const&) = delete;
	void operator=(UserInterfaceManager const&) = delete;

private:
	UserInterfaceManager() {}
};

#endif