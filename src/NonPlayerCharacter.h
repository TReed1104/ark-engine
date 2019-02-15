#ifndef ARKENGINE_NONPLAYERCHARACYER_H_
#define ARKENGINE_NONPLAYERCHARACYER_H_

#include "Entity.h"


class NonPlayerCharacter : public Entity {
public:
	NonPlayerCharacter(const std::string& filePath);
	~NonPlayerCharacter(void);

private:
	virtual void EntityController(void);
};

#endif