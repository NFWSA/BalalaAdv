#ifndef __SHOOT_ENEMY_H__
#define __SHOOT_ENEMY_H__

#include "cocos2d.h"
#include "Player.h"
#include <string>
#include "Enemy.h"

class Player;

class ShootEnemy : public Enemy
{
public:

	virtual bool init();
	virtual void aiAction(float dt);

	virtual void setEnemyPos(const cocos2d::Vec2 &pos);

	CREATE_FUNC(ShootEnemy);
protected:

};

#endif //__SHOOT_ENEMY_H__
