#ifndef __BRANCH_ENEMY_H__
#define __BRANCH_ENEMY_H__

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include <string>

class Player;

class BranchEnemy : public Enemy
{
public:

	virtual bool init();

	virtual bool getDamage(float damage);
	virtual void aiAction(float dt);

	CREATE_FUNC(BranchEnemy);
protected:

};

#endif //__BRANCH_ENEMY_H__
