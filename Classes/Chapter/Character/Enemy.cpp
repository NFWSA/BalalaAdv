#include "Enemy.h"
#include "ui\CocosGUI.h"
#include <cmath>
#include "SimpleAudioEngine.h"
#include "../TypeDefine.h"

USING_NS_CC;
using namespace CocosDenshion;

const Vec2 Enemy::pos2coor(const Vec2 &pos)
{
	if (nullptr == _map)
		return Vec2(-1, -1);
	Vec2 tar(_map->convertToNodeSpace(pos));
	int x = tar.x / _map->getTileSize().width, y = (_map->getTileSize().height*_map->getMapSize().height - tar.y) / _map->getTileSize().height;
	if (x < 0 || x >= _map->getMapSize().width)
		return Vec2(-1, -1);
	if (y < 0 || y >= _map->getMapSize().height)
		return Vec2(-1, -1);
	return Vec2(x, y);
}

void Enemy::setEnemyPos(const Vec2 &pos) {
	auto tar = _map->convertToWorldSpace(pos);
	if (pos.x > -_enemy->getContentSize().width - 1 && pos.x < _map->getTileSize().width*_map->getMapSize().width + _enemy->getContentSize().width + 1){
		setPosition(tar);
		_pos = pos;
	}
	if (nullptr != _player&&getEnemyBox().intersectsRect(_player->getBoundingBox())){
		if (_player->getDamage(30))
			SimpleAudioEngine::getInstance()->playEffect(_attEffect.c_str());
	}
}

void Enemy::aiAction(float dt)
{
	setEnemyPos(_pos);

	if (_paused)
		return;

	auto hdelta = _map->convertToNodeSpace(_player->getPosition()).y - _pos.y;
	if (hdelta >= std::abs(_player->getContentSize().height - _enemy->getContentSize().height) / 2)
		onJumpBegan();
	if (_aiChecked)
		return;
	auto dis = _pos.x - _map->convertToNodeSpace(_player->getPosition()).x;
	if (Director::getInstance()->getVisibleSize().width < std::abs(dis))
		return;
	_aiChecked = true;
	dis > 0 ? toLeft(dt) : toRight(dt);
}

void Enemy::setShoot(bool flag)
{
	_stopShoot = flag;
}

void Enemy::onShootBegan()
{
	_aiChecked = false;
	if (_stopShoot)
		return;
	if (!_canShoot)
		return;
	_canShoot = false;
	schedule(schedule_selector(Enemy::unlockShoot), 2.0f);
	auto ball = EnemyBall::create();
	ball->setType(_ballTexture[_ballTexture.size() - 1] == 'g' ? EnemyBall::TEXTURE_BALL : EnemyBall::PARTICLE_BALL);
	ball->setTexture(_ballTexture);
	getParent()->addChild(ball);
	ball->setPosition(getPosition());
	ball->setTarget(_player->getPosition(), _map, _player);
	ball->setProper(0, 0, 8, 10);
	_balls.pushBack(ball);

	SimpleAudioEngine::getInstance()->playEffect(_attEffect.c_str());
	schedule(schedule_selector(Enemy::onShoot), 0.05f);
}

void Enemy::onShoot(float dt)
{
	if (_balls.empty()){
		return;
	}
	if (_stopShoot){
		for (auto i : _balls)
			getParent()->removeChild(i);
		_balls.clear();
		return;
	}
	cocos2d::Vector<EnemyBall*> deadBalls;
	for (auto i : _balls){
		if (i->shoot(dt)){
			deadBalls.pushBack(i);
		}
	}
	for (auto i : deadBalls){
		_balls.eraseObject(i, true);
	}
}

void Enemy::toLeft(float dt)
{
	if (isScheduled(schedule_selector(Enemy::toLeft))){
		onLeftEnded();
		unschedule(schedule_selector(Enemy::toLeft));
		_aiChecked = false;
	}
	else{
		onLeftBegan();
		schedule(schedule_selector(Enemy::toLeft), 1.0f);
	}
}

void Enemy::toRight(float dt)
{
	if (isScheduled(schedule_selector(Enemy::toRight))){
		onRightEnded();
		unschedule(schedule_selector(Enemy::toRight));
		_aiChecked = false;
	}
	else{
		onRightBegan();
		schedule(schedule_selector(Enemy::toRight), 1.0f);
	}
}

void Enemy::setPause(bool flag)
{
	_paused = flag;
}

bool Enemy::init()
{
	if (!Node::init()){
		return false;
	}
	_health = _maxHealth = 50.0;
	_jumpHeight = 0;
	_enemyJumped = _aiChecked = _paused = _stopShoot = false;
	_canGetMag = _canShoot = true;
	_jumpSpeed = 8;
	_speed = 2;

	_player = nullptr;
	_map = nullptr;
	_hpBox = _enemy = nullptr;
	_hpBar = nullptr;

	_hpBar = ProgressTimer::create(Sprite::create("Character/EnemyHPBar.png"));
	_hpBar->setType(ProgressTimer::Type::BAR);
	_hpBar->setMidpoint(Vec2(0, 0.5f));
	_hpBar->setBarChangeRate(Vec2(1, 0));
	_hpBar->setPercentage(100.0);
	addChild(_hpBar);
	_hpBox = Sprite::create("Character/EnemyHPBox.png");
	addChild(_hpBox);

	_onLeftSel = schedule_selector(Enemy::onLeft);
	_onRightSel = schedule_selector(Enemy::onRight);
	_onJumpSel = schedule_selector(Enemy::onJump);
	_onFallSel = schedule_selector(Enemy::onFall);
	_onDropSel = schedule_selector(Enemy::onDrop);
	_onAISel = schedule_selector(Enemy::aiAction);

	return true;
}

void Enemy::setTMXMap(MapType *map, ValueMap &pos)
{
	_map = map;
	if (!pos.empty()){
		_pos = Vec2(pos["x"].asInt(), pos["y"].asInt());
		auto tar = Vec2(_pos.x, _pos.y + _enemy->getContentSize().height / 2);
		setEnemyPos(tar);
		schedule(_onDropSel, 0.0125f);
	}
}

void Enemy::setTexture(const std::string &str)
{
	_enemy = Sprite::create("Character/" + str);
	addChild(_enemy);
	_hpBox->setScaleX(_enemy->getContentSize().width / _hpBox->getContentSize().width);
	_hpBar->setScaleX(_enemy->getContentSize().width / _hpBar->getContentSize().width);
	_hpBox->setPositionY((_hpBox->getContentSize().height + _enemy->getContentSize().height) / 2);
	_hpBar->setPositionY((_hpBar->getContentSize().height + _enemy->getContentSize().height) / 2);
}

void Enemy::setBallTexture(const std::string &str)
{
	_ballTexture = str;
}

void Enemy::setPlayer(Player *player)
{
	_player = player;
}

void Enemy::unlockShoot(float dt)
{
	unschedule(schedule_selector(Enemy::unlockShoot));
	_canShoot = true;
}

void Enemy::unlockMagicDam(float dt)
{
	unschedule(schedule_selector(Enemy::unlockMagicDam));
	_canGetMag = true;
}

bool Enemy::getDamage(float damage)
{
	_health -= damage;
	if (MagDamage == damage){
		if (!_canGetMag){
			_health += damage;
			return false;
		}
		_canGetMag = false;
		schedule(schedule_selector(Enemy::unlockMagicDam), 0.8f);
	}
	if (_health > _maxHealth)
		_health = _maxHealth;
	setHP(_health / _maxHealth * 100.0f);
	if (_health <= 0){
		SimpleAudioEngine::getInstance()->playEffect(_deadEffect.c_str());
		for (auto i : _balls){
			getParent()->removeChild(i);
		}
		_balls.clear();
		getParent()->removeChild(this);
		return true;
	}
	return false;
}

void Enemy::setEffect(std::string &att, std::string &dead)
{
	_attEffect = att;
	_deadEffect = dead;
}

void Enemy::setHP(float hp)
{
	_hpBar->setPercentage(hp);
}

Rect Enemy::getEnemyBox() const
{
	auto rect = _enemy->getBoundingBox();
	rect.origin += getPosition();
	return rect;
}

bool Enemy::isTarDisenabled(const Vec2 &tar)
{
	if (nullptr == _map)
		return false;

	if (_pos.x == tar.x){
		Vec2 newTar(_map->convertToWorldSpace(tar));
		auto tarBeg = pos2coor(Vec2(getPositionX() - _enemy->getContentSize().width / 2 + 1,
														newTar.y + (newTar.y > getPositionY() ? _enemy->getContentSize().height / 2 : -_enemy->getContentSize().height / 2)));
		int beg = tarBeg.x, cexp = tarBeg.y;
		if (cexp<0 || cexp>_map->getMapSize().height*_map->getTileSize().height)
			return true;
		int end = pos2coor(Vec2(getPositionX() + _enemy->getContentSize().width / 2 - 1,
												newTar.y + (newTar.y > getPositionY() ? _enemy->getContentSize().height / 2 : -_enemy->getContentSize().height / 2))).x;
		auto meta = _map->getLayer("Meta");
		for (int i = beg; i <= end; ++i){
			int gid = meta->getTileGIDAt(Vec2(i, cexp));
			if (gid){
				auto proper = _map->getPropertiesForGID(gid).asValueMap();
				if (!proper.empty()){
					if ("True" == proper["Collision"].asString())
						return true;
				}
			}
		}
		return false;
	}

	if (_pos.y == tar.y){
		Vec2 newTar(_map->convertToWorldSpace(tar));
		auto tarBeg = pos2coor(Vec2(newTar.x + (newTar.x>getPositionX() ? _enemy->getContentSize().width / 2 : -_enemy->getContentSize().width / 2),
														getPositionY() + _enemy->getContentSize().height / 2 - 1));
		int beg = tarBeg.y, cexp = tarBeg.x;
		if (cexp<0 || cexp>_map->getMapSize().width*_map->getTileSize().width)
			return true;
		int end = pos2coor(Vec2(newTar.x + (newTar.x>getPositionX() ? _enemy->getContentSize().width / 2 : -_enemy->getContentSize().width / 2),
												getPositionY() - _enemy->getContentSize().height / 2 + 1)).y;
		auto meta = _map->getLayer("Meta");
		for (int i = beg; i <= end; ++i){
			int gid = meta->getTileGIDAt(Vec2(cexp, i));
			if (gid){
				auto proper = _map->getPropertiesForGID(gid).asValueMap();
				if (!proper.empty()){
					if ("True" == proper["Collision"].asString())
						return true;
				}
			}
		}
		return false;
	}

	return false;
}

void Enemy::onUpBegan()
{
	//schedule(schedule_selector(Enemy::onUp), 100);
	//do something
}

void Enemy::onDownBegan()
{
	//do something
}

void Enemy::onLeftBegan()
{
	schedule(_onLeftSel, 0.0125f);
	_enemy->setFlippedX(true);
}

void Enemy::onRightBegan()
{
	schedule(_onRightSel, 0.0125f);
	_enemy->setFlippedX(false);
}

void Enemy::onJumpBegan()
{
	if (_enemyJumped)
		return;
	_jumpHeight = 0;
	_enemyJumped = true;
	schedule(_onJumpSel, 0.0125f);
}

void Enemy::onUp(float dt)
{
	//do something
}

void Enemy::onDown(float dt)
{
	//do something
}

void Enemy::onJump(float dt)
{
	for (int i = _jumpSpeed; i > 0; --i){
		Vec2 tar(_pos.x, _pos.y + i);
		if (isTarDisenabled(tar)){
			continue;
		}
		setEnemyPos(tar);
		_jumpHeight += i;
		if (160 <= _jumpHeight){
			unschedule(_onJumpSel);
			schedule(_onFallSel, 0.0125f);
		}
		return;
	}
	unschedule(_onJumpSel);
	schedule(_onFallSel, 0.0125f);
}

void Enemy::onLeft(float dt)
{
	for (int i = _speed; i > 0; --i){
		Vec2 tar(_pos.x - i, _pos.y);
		if (isTarDisenabled(tar))
			continue;
		setEnemyPos(tar);
		break;
	}
	if (!_enemyJumped && !isScheduled(_onJumpSel) && !isScheduled(_onFallSel))
		schedule(_onDropSel, 0.0125f);
}

void Enemy::onRight(float dt)
{
	for (int i = _speed; i > 0; --i){
		Vec2 tar(_pos.x + i, _pos.y);
		if (isTarDisenabled(tar))
			continue;
		setEnemyPos(tar);
		break;
	}
	if (!_enemyJumped && !isScheduled(_onFallSel))
		schedule(_onDropSel, 0.0125f);
}

void Enemy::onUpEnded()
{
	//do something
}

void Enemy::onDownEnded()
{
	//do something
}

void Enemy::onLeftEnded()
{
	unschedule(_onLeftSel);
}

void Enemy::onRightEnded()
{
	unschedule(_onRightSel);
}

void Enemy::onJumpEnded()
{
	unschedule(_onJumpSel);
	if (isScheduled(_onFallSel))
		return;
	schedule(_onFallSel, 0.0125f);
}

void Enemy::onFall(float dt)
{
	for (int i = _jumpSpeed; i > 0; --i){
		Vec2 tar(_pos.x, _pos.y - i);
		if (isTarDisenabled(tar))
			continue;
		setEnemyPos(tar);
		return;
	}
	_enemyJumped = false;
	unschedule(_onFallSel);
}

void Enemy::onDrop(float dt)
{
	for (int i = _jumpSpeed; i > 0; --i){
		Vec2 tar(_pos.x, _pos.y - i);
		if (isTarDisenabled(tar))
			continue;
		setEnemyPos(tar);
		return;
	}
	unschedule(_onDropSel);
}

bool EnemyBall::init()
{
	if (!Node::init())
		return false;

	_partBall = nullptr;
	_texBall = nullptr;

	_dy = _da = 0;
	_dSpeed = 8;
	_ignoreBlock = _crossPlayer = false;

	return true;
}

Rect EnemyBall::getBallBox()
{
	auto rect = PARTICLE_BALL == _ballType ? _partBall->getBoundingBox() : _texBall->getBoundingBox();
	rect.origin += getPosition();
	return rect;
}

bool EnemyBall::shoot(float dt)
{
	_dy += dt * _da;
	Vec2 delta(_nor * _dSpeed);
	delta.y -= _dy;
	_now += delta;
	setRotation(-delta.getAngle() * 180 / std::_Pi);
	setPosition(_map->convertToWorldSpace(_now));

	if (_player->getPlayerBox().intersectsRect(getBallBox())){
		_player->getDamage(_dDamage);
		if (!_crossPlayer){
			getParent()->removeChild(this);
			return true;
		}
	}

	if (_now.y < 0 || _now.y > Director::getInstance()->getVisibleSize().height * 2 || isBlockDisabled(_now)){
		getParent()->removeChild(this);
		return true;
	}
	return false;
}

void EnemyBall::setType(int type)
{
	_ballType = type;
}

void EnemyBall::setTarget(const cocos2d::Vec2 &tar, MapType *map, Player *player)
{
	_map = map;
	_now = _map->convertToNodeSpace(getPosition());
	_nor = _map->convertToNodeSpace(tar) - _now;
	_nor.normalize();
	_player = player;
}

void EnemyBall::setTexture(const std::string &str)
{
	if (PARTICLE_BALL == _ballType){
		auto part = ParticleSystemQuad::create(str);
		_partBall = ParticleBatchNode::createWithTexture(part->getTexture());
		_partBall->addChild(part);
		addChild(_partBall);
		part->setPosition(0, 0);
	}
	else{
		_texBall = Sprite::create(str);
		addChild(_texBall);
	}
}

void EnemyBall::setProper(float dy, float da, float speed, float damage)
{
	_dy = dy;
	_da = da;
	_dSpeed = speed;
	_dDamage = damage;
}

void EnemyBall::setIgnoreBlock(bool flag)
{
	_ignoreBlock = flag;
}

void EnemyBall::setCrossPlayer(bool flag)
{
	_crossPlayer = flag;
}

bool EnemyBall::isBlockDisabled(const cocos2d::Vec2 &tar)
{
	if (nullptr == _map)
		return true;
	int x = tar.x / _map->getTileSize().width, y = (_map->getTileSize().height*_map->getMapSize().height - tar.y) / _map->getTileSize().height;
	if (x < 0 || x >= _map->getMapSize().width)
		return true;
	if (y < 0 || y >= _map->getMapSize().height)
		return false;
	if (_ignoreBlock)
		return false;
	int gid = _map->getLayer("Meta")->getTileGIDAt(Vec2(x, y));
	if (gid){
		auto proper = _map->getPropertiesForGID(gid).asValueMap();
		if (!proper.empty()){
			if ("True" == proper["Collision"].asString())
				return true;
		}
	}
	return false;
}
