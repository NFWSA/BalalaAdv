#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include <functional>
#include "Enemy.h"
#include "../TypeDefine.h"

class ShootBall;
class Enemy;

class Player : public cocos2d::Node
{
public:
	enum MODE{PHYATT = 1, HLPATT, MAGATT};

	const cocos2d::Vec2 pos2coor(const cocos2d::Vec2 &pos);
	void setPlayerPos(const cocos2d::Vec2 &poss);

	virtual bool init();
	void onUpBegan();
	void onDownBegan();
	void onLeftBegan();
	void onRightBegan();
	void onJumpBegan();
	void onUp(float dt);
	void onDown(float dt);
	void onLeft(float dt);
	void onRight(float dt);
	void onJump(float dt);
	void onFall(float dt);
	void onDrop(float dt);
	void onUpEnded();
	void onDownEnded();
	void onLeftEnded();
	void onRightEnded();
	void onJumpEnded();

	bool onMode1();
	bool onMode2();
	bool onMode3();
	void onClickMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void onClickEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	bool isTarDisenabled(const cocos2d::Vec2 &tar);

	cocos2d::Rect getPlayerBox();

	bool getDamage(float damage);
	void healAtt(float dt);
	void unlockShoot(float dt);
	void unlockMagic(float dt);
	void unlockGetDam(float dt);
	void attAtt(float dt);
	void setEnemyShoot(bool flag);

	void setTMXMap(MapType *map);
	void setDamRate(const double damRate);

	CREATE_FUNC(Player);

	std::function<void(float)> setHP;
	cocos2d::Vector<Enemy*> _enemys;
private:
	cocos2d::Sprite *_player;
	double _health, _maxHealth, _damRate;
	int _mode, _jumpHeight;
	unsigned int _hlpEffect;
	bool _playerJumped;
	MapType *_map;
	cocos2d::ParticleBatchNode *_healRain;
	cocos2d::Vector<ShootBall*> _balls;
	bool _isShooting, _isMagicking, _canGetDam;
	cocos2d::Sequence *_jumpAct, *_revJumpAct, *_flashAct;
};

class ShootBall : public cocos2d::Node
{
public:
	virtual bool init();

	bool shoot(float dt);
	void setType(int type);
	void setTarget(const cocos2d::Vec2 &tar, MapType *map);

	cocos2d::Rect getBallBox();

	bool isEnemyDead(Enemy *enemy);
	bool isDead();
	bool isBlockDisabled(const cocos2d::Vec2 &tar);

	CREATE_FUNC(ShootBall);
private:
	cocos2d::ParticleBatchNode *_ball;
	int _attType;
	float _dy;
	bool _dead;
	cocos2d::Vec2 _tar, _nor, _now;
	MapType *_map;
};

#endif //__PLAYER_H__
