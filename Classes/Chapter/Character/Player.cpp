#include "Player.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Rect Player::getPlayerBox()
{
	auto rect = _player->getBoundingBox();
	rect.origin += getPosition();
	return rect;
}

void Player::setEnemyShoot(bool flag)
{
	for (auto i : _enemys){
		i->setShoot(flag);
	}
}

const Vec2 Player::pos2coor(const Vec2 &pos)
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

void Player::setPlayerPos(const Vec2 &pos) {
	auto gid = _map->getLayer("Meta")->getTileGIDAt(pos2coor(pos));
	if (gid){
		auto proper = _map->getPropertiesForGID(gid).asValueMap();
		if (!proper.empty()){
			if ("True" == proper["Pass"].asString()){
				_eventDispatcher->dispatchCustomEvent("GamePass");
			}
			if ("True" == proper["WillDead"].asString()){
				_eventDispatcher->dispatchCustomEvent("GameOver");
			}
		}
	}
	if (pos.x >= Director::getInstance()->getVisibleSize().width / 4 && pos.x <= Director::getInstance()->getVisibleSize().width * 3 / 4){
		setPosition(pos);
		return;
	}
	if (pos.x < Director::getInstance()->getVisibleSize().width / 4){
		if (_map->getPositionX() - pos.x + Director::getInstance()->getVisibleSize().width / 4 > 0){
			if (pos.x >= 0 && pos.x <= Director::getInstance()->getVisibleSize().width)
				setPosition(pos);
		}
		else{
			setPositionX(Director::getInstance()->getVisibleSize().width / 4);
			_map->setPositionX(_map->getPositionX() + Director::getInstance()->getVisibleSize().width / 4 - pos.x);
		}
		return;
	}
	if (pos.x > Director::getInstance()->getVisibleSize().width * 3 / 4){
		if (-_map->getPositionX() + pos.x - Director::getInstance()->getVisibleSize().width * 3 / 4 > _map->getMapSize().width*_map->getTileSize().width-Director::getInstance()->getVisibleSize().width){
			if (pos.x >= 0 && pos.x <= Director::getInstance()->getVisibleSize().width)
				setPosition(pos);
		}
		else{
			setPositionX(Director::getInstance()->getVisibleSize().width * 3 / 4);
			_map->setPositionX(_map->getPositionX() - pos.x + Director::getInstance()->getVisibleSize().width * 3 / 4);
		}
		return;
	}

}

bool Player::init()
{
	if (!Node::init()){
		return false;
	}

	_player = Sprite::create("Character/player1.png");
	addChild(_player);
	_health = _maxHealth = 100.0;
	_mode = PHYATT;
	_map = nullptr;
	_playerJumped = false;
	_canGetDam = true;
	_jumpHeight = 0;
	setHP = nullptr;
	_healRain = nullptr;
	_damRate = 1.0f;

	auto delay = DelayTime::create(0.1f);
	auto rotate = RotateBy::create(0.2f, 360.0);
	_jumpAct = Sequence::create(delay, rotate, nullptr);
	_jumpAct->retain();
	_revJumpAct = Sequence::create(delay->clone(), rotate->reverse(), nullptr);
	_revJumpAct->retain();

	auto flash1 = FadeTo::create(0.25f, 80), flash2 = FadeTo::create(0.25f, 255);
	_flashAct = Sequence::create(flash1, flash2, flash1->clone(), flash2->clone(), nullptr);
	_flashAct->retain();

	_isShooting = _isMagicking = false;

	schedule(schedule_selector(Player::attAtt), 0.0125f);

	return true;
}

void Player::onUpBegan()
{
	//schedule(schedule_selector(Player::onUp), 100);
	//do something
}

void Player::onDownBegan()
{
	//do something
}

void Player::onLeftBegan()
{
	schedule(schedule_selector(Player::onLeft), 0.0125f);
	_player->setFlippedX(true);
}

void Player::onRightBegan()
{
	schedule(schedule_selector(Player::onRight), 0.0125f);
	_player->setFlippedX(false);
}

void Player::onJumpBegan()
{
	if (_playerJumped)
		return;
	_jumpHeight = 0;
	_playerJumped = true;
	schedule(schedule_selector(Player::onJump), 0.0125f);
	_player->runAction(_player->isFlippedX() ? _revJumpAct : _jumpAct);
	//do something
}

void Player::onUp(float dt)
{
	//do something
}

void Player::onDown(float dt)
{
	//do something
}

void Player::onJump(float dt)
{
	Vec2 tar(getPositionX(), getPositionY());
	for (int i = 8; i > 0; --i){
		Vec2 newTar(tar.x, tar.y + i);
		if (isTarDisenabled(newTar)){
			continue;
		}
		setPlayerPos(newTar);
		_jumpHeight += i;
		if (160 <= _jumpHeight){
			unschedule(schedule_selector(Player::onJump));
			schedule(schedule_selector(Player::onFall), 0.0125f);
		}
		return;
	}
	unschedule(schedule_selector(Player::onJump));
	schedule(schedule_selector(Player::onFall), 0.0125f);
	_player->stopAction(_jumpAct);
	_player->stopAction(_revJumpAct);
	_player->setRotation(0);
}

void Player::onLeft(float dt)
{
	Vec2 tar(getPositionX(), getPositionY());
	for (int i = 4; i > 0; --i){
		Vec2 newTar(tar.x - i, tar.y);
		if (isTarDisenabled(newTar))
			continue;
		setPlayerPos(newTar);
		break;
	}
	if (!_playerJumped && !isScheduled(schedule_selector(Player::onJump)) && !isScheduled(schedule_selector(Player::onFall)))
		schedule(schedule_selector(Player::onDrop), 0.0125f);
}

void Player::onRight(float dt)
{
	Vec2 tar(getPositionX(), getPositionY());
	for (int i = 4; i > 0; --i){
		Vec2 newTar(tar.x + i, tar.y);
		if (isTarDisenabled(newTar))
			continue;
		setPlayerPos(newTar);
		break;
	}
	if (!_playerJumped && !isScheduled(schedule_selector(Player::onFall)))
		schedule(schedule_selector(Player::onDrop), 0.0125f);
}

void Player::onUpEnded()
{
	//do something
}

void Player::onDownEnded()
{
	//do something
}

void Player::onLeftEnded()
{
	unschedule(schedule_selector(Player::onLeft));
}

void Player::onRightEnded()
{
	unschedule(schedule_selector(Player::onRight));
}

void Player::onJumpEnded()
{
	unschedule(schedule_selector(Player::onJump));
	if (isScheduled(schedule_selector(Player::onFall)))
		return;
	schedule(schedule_selector(Player::onFall), 0.0125f);
	_player->stopAction(_jumpAct);
	_player->stopAction(_revJumpAct);
	_player->setRotation(0);
}

void Player::onFall(float dt)
{
	Vec2 tar(getPositionX(), getPositionY());
	for (int i = 8; i > 0; --i){
		Vec2 newTar(tar.x, tar.y - i);
		if (isTarDisenabled(newTar))
			continue;
		setPlayerPos(newTar);
		return;
	}
	_playerJumped = false;
	unschedule(schedule_selector(Player::onFall));
}

void Player::onDrop(float dt)
{
	Vec2 tar(getPositionX(), getPositionY());
	for (int i = 8; i > 0; --i){
		Vec2 newTar(tar.x, tar.y - i);
		if (isTarDisenabled(newTar))
			continue;
		setPlayerPos(newTar);
		return;
	}
	unschedule(schedule_selector(Player::onDrop));
}

bool Player::onMode1()
{
	_mode = PHYATT;
	_player->setTexture("Character/Player1.png");
	_healRain->setVisible(false);
	SimpleAudioEngine::getInstance()->stopEffect(_hlpEffect);
	if (isScheduled(schedule_selector(Player::healAtt)))
		unschedule(schedule_selector(Player::healAtt));
	return true;
}

bool Player::onMode2()
{
	_mode = HLPATT;
	_player->setTexture("Character/Player2.png");
	return true;
}

bool Player::onMode3()
{
	_mode = MAGATT;
	_player->setTexture("Character/Player3.png");
	_healRain->setVisible(false);
	SimpleAudioEngine::getInstance()->stopEffect(_hlpEffect);
	if (isScheduled(schedule_selector(Player::healAtt)))
		unschedule(schedule_selector(Player::healAtt));
	return true;
}

void Player::onClickMoved(Touch *touch, Event *event)
{
	if (HLPATT == _mode){
		_healRain->setVisible(true);
		_hlpEffect = SimpleAudioEngine::getInstance()->playEffect("Sound/Effect/02.wav", true);
		_healRain->setPosition(touch->getLocation());
		if (!isScheduled(schedule_selector(Player::healAtt))){
			schedule(schedule_selector(Player::healAtt), 0.5f);
		}
	}
	else{
		return;
	}
}

void Player::onClickEnded(Touch *touch, Event *event)
{
	if (HLPATT == _mode){
		_healRain->setVisible(false);
		SimpleAudioEngine::getInstance()->stopEffect(_hlpEffect);
		unschedule(schedule_selector(Player::healAtt));
		return;
	}
	if (MAGATT == _mode){
		if (_isMagicking)
			return;
		_isMagicking = true;
		schedule(schedule_selector(Player::unlockMagic), 1.5f);

		auto ball = ShootBall::create();
		ball->setType(MAGATT);
		getParent()->addChild(ball);
		ball->setPosition(getPosition());
		ball->setTarget(touch->getLocation(), _map);
		_balls.pushBack(ball);
		SimpleAudioEngine::getInstance()->playEffect("Sound/Effect/03.wav");
		return;
	}
	if (PHYATT == _mode){
		if (_isShooting)
			return;
		_isShooting = true;
		schedule(schedule_selector(Player::unlockShoot), 0.25f);

		auto ball = ShootBall::create();
		ball->setType(PHYATT);
		getParent()->addChild(ball);
		ball->setPosition(getPosition());
		ball->setTarget(touch->getLocation(), _map);
		_balls.pushBack(ball);
		SimpleAudioEngine::getInstance()->playEffect("Sound/Effect/01.wav");
		return;
	}
}

void Player::unlockShoot(float dt)
{
	unschedule(schedule_selector(Player::unlockShoot));
	_isShooting = false;
}

void Player::unlockMagic(float dt)
{
	unschedule(schedule_selector(Player::unlockMagic));
	_isMagicking = false;
}

void Player::attAtt(float dt)
{
	if (_balls.empty())
		return;
	Vector<ShootBall*> deadBalls;
	Vector<Enemy*> deadEnemys;
	for (auto i : _balls){
		if (i->shoot(dt)){
			deadBalls.pushBack(i);
			continue;
		}
		for (auto j : _enemys){
			if (i->isEnemyDead(j)){
				deadEnemys.pushBack(j);
			}
		}
		for (auto j : deadEnemys){
			_enemys.eraseObject(j, true);
		}
		deadEnemys.clear();
		if (i->isDead())
			deadBalls.pushBack(i);
	}
	for (auto i : deadBalls)
		_balls.eraseObject(i, true);
	for (auto i : deadEnemys)
		_enemys.eraseObject(i, true);
}

void Player::healAtt(float dt)
{
	if (getPlayerBox().intersectsRect(_healRain->getBoundingBox()))
		getDamage(HlpDamage / 2);
	Vector<Enemy*> deadEnemy;
	for (auto i : _enemys){
		if (i->getEnemyBox().intersectsRect(_healRain->getBoundingBox())){
			if (i->getDamage(HlpDamage)){
				deadEnemy.pushBack(i);
			}
		}
	}
	for (auto i : deadEnemy){
		_enemys.eraseObject(i, true);
	}
}

bool Player::isTarDisenabled(const Vec2 &tar)
{
	if (nullptr == _map)
		return false;

	if (getPositionX() == tar.x){
		auto tarBeg = pos2coor(Vec2(getPositionX() - _player->getContentSize().width / 2 + 1,
														tar.y + (tar.y > getPositionY() ? _player->getContentSize().height / 2 : -_player->getContentSize().height / 2)));
		int beg = tarBeg.x, cexp = tarBeg.y;
		if (cexp<0 || cexp>_map->getMapSize().height*_map->getTileSize().height)
			return true;
		int end = pos2coor(Vec2(getPositionX() + _player->getContentSize().width / 2 - 1,
												tar.y + (tar.y > getPositionY() ? _player->getContentSize().height / 2 : -_player->getContentSize().height / 2))).x;
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

	if (getPositionY() == tar.y){
		auto tarBeg = pos2coor(Vec2(tar.x + (tar.x>getPositionX() ? _player->getContentSize().width / 2 : -_player->getContentSize().width / 2),
														getPositionY() + _player->getContentSize().height / 2 - 1));
		int beg = tarBeg.y, cexp = tarBeg.x;
		if (cexp<0 || cexp>_map->getMapSize().width*_map->getTileSize().width)
			return true;
		int end = pos2coor(Vec2(tar.x + (tar.x>getPositionX() ? _player->getContentSize().width / 2 : -_player->getContentSize().width / 2),
												getPositionY() -  _player->getContentSize().height / 2 + 1)).y;
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

void Player::unlockGetDam(float dt)
{
	unschedule(schedule_selector(Player::unlockGetDam));
	_canGetDam = true;
}

void Player::setDamRate(const double damRate)
{
	_damRate = damRate;
}

bool Player::getDamage(float damage)
{
	bool flag = false;
	if (damage > 0){
		if (!_canGetDam)
			return flag;
		_canGetDam = false;
		_player->runAction(_flashAct);
		flag = true;
		schedule(schedule_selector(Player::unlockGetDam), 1.0f);
		_health -= damage * _damRate;
	}
	else
		_health -= damage;
	if (_health > _maxHealth)
		_health = _maxHealth;
	if (nullptr != setHP)
		setHP(_health / _maxHealth * 100.0f);
	if (_health <= 0.0){
		SimpleAudioEngine::getInstance()->stopEffect(_hlpEffect);
		EventCustom event("GameOver");
		_jumpAct->release();
		_revJumpAct->release();
		_flashAct->release();
		_eventDispatcher->dispatchEvent(&event);
	}
	return flag;
}

void Player::setTMXMap(MapType *map)
{
	_map = map;
	auto pos = _map->getObjectGroup("Objects")->getObject("Player");
	if (!pos.empty()){
		setPosition(Vec2(pos["x"].asInt(), pos["y"].asInt() + _player->getContentSize().height/2));
		schedule(schedule_selector(Player::onDrop), 0.0125f);
	}

	auto part = ParticleSystemQuad::create("UI/HealRain.plist");
	_healRain = ParticleBatchNode::createWithTexture(part->getTexture());
	_healRain->addChild(part);
	part->setPosition(0, 0);
	_healRain->setVisible(false);
	getParent()->addChild(_healRain, 5);
}

bool ShootBall::init()
{
	if (!Node::init())
		return false;

	_dead = false;
	_dy = 0;

	return true;
}

bool ShootBall::shoot(float dt)
{
	if (Player::MAGATT == _attType){
		setPosition(_map->convertToWorldSpace(_now));
		Vec2 delta(_tar - _now);
		delta = delta.getClampPoint(_nor, _tar - _now + _nor*0.5f);
		delta = delta.length() < (_nor * 4).length() ? delta : _nor * 4;
		_now += delta;
		setPosition(_map->convertToWorldSpace(_now));

		if (_now.distance(_tar) < 0.5f){
			getParent()->removeChild(this);
			return true;
		}
	}
	else{
		_dy += dt * 4;
		Vec2 delta(_nor*8);
		_now += delta;
		_now.y -= _dy;
		setPosition(_map->convertToWorldSpace(_now));

		if ( _now.y < 32.0f || isBlockDisabled(_now)){
			getParent()->removeChild(this);
			return true;
		}
	}
	return false;
}

void ShootBall::setType(int type)
{
	_attType = type;
	if (Player::PHYATT == type){
		auto ball = ParticleSystemQuad::create("UI/PhyBal.plist");
		_ball = ParticleBatchNode::createWithTexture(ball->getTexture());
		_ball->addChild(ball);
		addChild(_ball);
		ball->setPosition(0, 0);
	}
	else{
		auto ball = ParticleSystemQuad::create("UI/BlkMag.plist");
		_ball = ParticleBatchNode::createWithTexture(ball->getTexture());
		_ball->addChild(ball);
		addChild(_ball);
		ball->setPosition(0, 0);
	}
}

void ShootBall::setTarget(const cocos2d::Vec2 &tar, MapType *map)
{
	_map = map;
	_tar = _map->convertToNodeSpace(tar);
	_now = _map->convertToNodeSpace(getPosition());
	_nor = _tar - _now;
	_nor.normalize();
}

bool ShootBall::isBlockDisabled(const Vec2 &tar)
{
	if (nullptr == _map)
		return true;
	int x = tar.x / _map->getTileSize().width, y = (_map->getTileSize().height*_map->getMapSize().height - tar.y) / _map->getTileSize().height;
	if (x < 0 || x >= _map->getMapSize().width)
		return false;
	if (y < 0 || y >= _map->getMapSize().height)
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

bool ShootBall::isDead()
{
	if (Player::PHYATT == _attType){
		if (_dead){
			getParent()->removeChild(this);
		}
		return _dead;
	}
	return false;
}

Rect ShootBall::getBallBox()
{
	auto rect = _ball->getBoundingBox();
	rect.origin += getPosition();
	return rect;
}

bool ShootBall::isEnemyDead(Enemy *enemy)
{ 
	if (enemy->getEnemyBox().intersectsRect(getBallBox())){
		_dead = true;
		return enemy->getDamage(Player::PHYATT == _attType ? PhyDamage : MagDamage);
	}
	return false;
}
