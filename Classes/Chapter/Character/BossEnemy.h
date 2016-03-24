#ifndef __BOSS_ENEMY_H__
#define __BOSS_ENEMY_H__

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include <string>

class Player;

class BossEnemy : public Enemy
{
public:

	virtual bool init();

	virtual bool getDamage(float damage);
	virtual void aiAction(float dt);
	virtual void setBallTexture(const std::string &str);
	virtual void setEnemyPos(const cocos2d::Vec2 &pos);

	CREATE_FUNC(BossEnemy);
protected:
	cocos2d::ValueMap _aiData;
	int _id;
	float _next;
};

#endif //__BOSS_ENEMY_H__
