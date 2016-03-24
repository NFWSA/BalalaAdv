#ifndef __FLYSHOOT_ENEMY_H__
#define __FLYSHOOT_ENEMY_H__

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include <string>

class Player;

class FlyShootEnemy : public Enemy
{
public:

	virtual bool init();
	virtual void onDrop(float dt);

	virtual void aiAction(float dt);

	virtual void setEnemyPos(const cocos2d::Vec2 &pos);

	CREATE_FUNC(FlyShootEnemy);
protected:

};

#endif //__FLYSHOOT_ENEMY_H__
