#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Player.h"
#include <string>
#include "../TypeDefine.h"

class Player;
class EnemyBall;

class Enemy : public cocos2d::Node
{
public:
	const cocos2d::Vec2 pos2coor(const cocos2d::Vec2 &pos);
	virtual void setEnemyPos(const cocos2d::Vec2 &pos);

	void onUpBegan();
	void onDownBegan();
	void onLeftBegan();
	void onRightBegan();
	void onJumpBegan();
	void onUp(float dt);
	void onDown(float dt);
	void onLeft(float dt);
	void onRight(float dt);
	virtual void onJump(float dt);
	virtual void onFall(float dt);
	virtual void onDrop(float dt);
	void onUpEnded();
	void onDownEnded();
	void onLeftEnded();
	void onRightEnded();
	void onJumpEnded();

	void onShootBegan();
	void onShoot(float dt);


	void toLeft(float dt);
	void toRight(float dt);

	void unlockMagicDam(float dt);
	void unlockShoot(float dt);
	virtual void aiAction(float dt);

	virtual bool init();

	bool isTarDisenabled(const cocos2d::Vec2 &tar);

	virtual bool getDamage(float damage);
	void setHP(float hp);
	void setPause(bool flag);

	virtual void setTMXMap(MapType *map, cocos2d::ValueMap &pos);
	void setTexture(const std::string &str);
	virtual void setBallTexture(const std::string &str);
	virtual void setPlayer(Player *player);
	void setEffect(std::string &att, std::string &dead);
	void setShoot(bool flag);

	cocos2d::Rect getEnemyBox() const;

	CREATE_FUNC(Enemy);

	cocos2d::SEL_SCHEDULE _onLeftSel, _onRightSel, _onJumpSel, _onFallSel, _onDropSel, _onAISel;
protected:
	float _health, _maxHealth;
	cocos2d::Sprite *_enemy, *_hpBox;
	cocos2d::Vec2 _pos;
	Player *_player;
	int _jumpHeight, _speed, _jumpSpeed;
	bool _enemyJumped, _canGetMag, _aiChecked, _canShoot, _paused, _stopShoot;
	std::string _attEffect, _deadEffect, _ballTexture;
	MapType *_map;
	cocos2d::ProgressTimer *_hpBar;
	cocos2d::Vector<EnemyBall*> _balls;
};

class EnemyBall : public cocos2d::Node
{
public:
	enum{ PARTICLE_BALL, TEXTURE_BALL };
	virtual bool init();

	bool shoot(float dt);
	void setType(int type);
	void setTarget(const cocos2d::Vec2 &tar, MapType *map, Player *player);
	void setTexture(const std::string &str);
	void setProper(float dy, float da, float speed, float damage); 
	void setIgnoreBlock(bool flag);
	void setCrossPlayer(bool flag);

	cocos2d::Rect getBallBox();

	bool isBlockDisabled(const cocos2d::Vec2 &tar);

	CREATE_FUNC(EnemyBall);
protected:
	Player *_player;
	cocos2d::ParticleBatchNode *_partBall;
	cocos2d::Sprite *_texBall;
	int _ballType;
	bool _ignoreBlock, _crossPlayer;
	float _dy, _da, _dSpeed, _dDamage;
	cocos2d::Vec2 _nor, _now;
	MapType *_map;
};

#endif //__ENEMY_H__
