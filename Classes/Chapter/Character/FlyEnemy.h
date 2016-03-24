#ifndef __FLY_ENEMY_H__
#define __FLY_ENEMY_H__

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include <string>

class Player;

class FlyEnemy : public Enemy
{
public:

	virtual bool init();
	virtual void onJump(float dt);
	virtual void onFall(float dt);
	virtual void onDrop(float dt);

	virtual void aiAction(float dt);

	CREATE_FUNC(FlyEnemy);
protected:

};

#endif //__FLY_ENEMY_H__
