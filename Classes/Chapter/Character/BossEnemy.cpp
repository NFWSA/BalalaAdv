#include "BossEnemy.h"
#include "ui\CocosGUI.h"
#include <cmath>
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

bool BossEnemy::init()
{
	if (!Enemy::init()){
		return false;
	}

	_id = 0;
	_health = _maxHealth = 600;
	_jumpSpeed = 24;
	_aiChecked = false;
	_next = 0;

	_onAISel = schedule_selector(Enemy::aiAction);

	return true;
}

void BossEnemy::aiAction(float dt)
{
	if (!_aiChecked)
		setEnemyPos(_pos);
	if (_paused){
		return;
	}
	_next -= dt;
	if (_next > 0)
		return;
	_aiChecked = false;
	if (!isScheduled(schedule_selector(Enemy::onShoot)))
		schedule(schedule_selector(Enemy::onShoot), 0.05f);

	if (_id == _aiData["number"].asInt())
		_id = 0;
	auto nowData = _aiData["action" + Value(_id).asString()].asValueMap();

	auto str = nowData["type"].asString();
	auto effect = nowData["effect"].asString();
	
	if ("U2D" == str || "D2U" == str || "L2R" == str || "R2L" == str){
		auto num = nowData["number"].asInt(), speed = nowData["speed"].asInt(), damage = nowData["damage"].asInt();
		auto texture = nowData["texture"].asString();
		float delta;
		Vec2 pos;
		if ("U2D" == str){
			delta = Director::getInstance()->getVisibleSize().width / (num - 1);
			pos = Vec2(0, Director::getInstance()->getVisibleSize().height);
		}
		if ("D2U" == str){
			delta = Director::getInstance()->getVisibleSize().width / (num - 1);
			pos = Vec2(0, 0);
		}
		if ("L2R" == str){
			delta = Director::getInstance()->getVisibleSize().height / (num - 1);
			pos = Vec2(0, 0);
		}
		if ("R2L" == str){
			delta = Director::getInstance()->getVisibleSize().height / (num - 1);
			pos = Vec2(Director::getInstance()->getVisibleSize().width, 0);
		}
		for (auto i = 0; i < num; ++i){
			auto ball = EnemyBall::create();
			ball->setType(texture[texture.size() - 1] == 'g' ? EnemyBall::TEXTURE_BALL : EnemyBall::PARTICLE_BALL);
			ball->setTexture(texture);
			ball->setIgnoreBlock(true);
			getParent()->addChild(ball);
			if ("U2D" == str){
				ball->setPosition(Vec2(pos.x + i*delta, pos.y));
				ball->setTarget(Vec2(pos.x + i*delta, pos.y - 1), _map, _player);
			}
			if ("D2U" == str){
				ball->setPosition(Vec2(pos.x + i*delta, pos.y));
				ball->setTarget(Vec2(pos.x + i*delta, pos.y + 1), _map, _player);
			}
			if ("L2R" == str){
				ball->setPosition(Vec2(pos.x, pos.y + i*delta));
				ball->setTarget(Vec2(pos.x + 1, pos.y + i*delta), _map, _player);
			}
			if ("R2L" == str){
				ball->setPosition(Vec2(pos.x, pos.y + i*delta));
				ball->setTarget(Vec2(pos.x - 1, pos.y + i*delta), _map, _player);
			}
			ball->setProper(0, 0, speed, damage);
			_balls.pushBack(ball);
		}
	}

	if ("P2F" == str){
		auto num = nowData["number"].asInt(), speed = nowData["speed"].asInt(), damage = nowData["damage"].asInt();
		auto texture = nowData["texture"].asString();
		auto cross = nowData["cross"].asBool();

		Vec2 sour = getPosition();
		Vec2 target = _player->getPosition().rotateByAngle(sour, -75 * std::_Pi / 180);

		auto delta = 150 / (num - 1);
		for (int i = 0; i < num; ++i){
			auto ball = EnemyBall::create();
			ball->setType(texture[texture.size() - 1] == 'g' ? EnemyBall::TEXTURE_BALL : EnemyBall::PARTICLE_BALL);
			ball->setTexture(texture);
			getParent()->addChild(ball);
			ball->setPosition(sour);
			ball->setIgnoreBlock(cross);
			ball->setTarget(target.rotateByAngle(sour, delta * i * std::_Pi / 180), _map, _player);
			ball->setProper(0, 0, speed, damage);
			_balls.pushBack(ball);
		}
	}
	if ("P2P" == str){
		auto speed = nowData["speed"].asInt(), damage = nowData["damage"].asInt();
		auto texture = nowData["texture"].asString();
		auto cross = nowData["cross"].asBool(), random = nowData["random"].asBool();
		auto ball = EnemyBall::create();
		ball->setType(texture[texture.size() - 1] == 'g' ? EnemyBall::TEXTURE_BALL : EnemyBall::PARTICLE_BALL);
		ball->setTexture(texture);
		ball->setIgnoreBlock(cross);
		getParent()->addChild(ball);
		ball->setPosition(getPosition());
		ball->setIgnoreBlock(cross);
		auto target = _player->getPosition();
		if (random)
			target = target.rotateByAngle(getPosition(), rand_0_1() * 2 * std::_Pi);
		ball->setTarget(target, _map, _player);
		ball->setProper(0, 0, speed, damage);
		_balls.pushBack(ball);
		
	}
	if ("MOVI" == str){
		_aiChecked = true;
		auto pos = _map->getObjectGroup("Objects")->getObject(nowData["name"].asString());
		setEnemyPos(Vec2(pos["x"].asInt(), pos["y"].asInt() + _enemy->getContentSize().height / 2));
		for (int i = 0; i < 7; ++i)
			onDrop(0);
		auto target = getPosition(), sour = target;
		auto from = pos["From"].asString();
		if ("Left" == from){
			sour.x = -_enemy->getContentSize().width;
		}
		if ("Right" == from){
			sour.x = Director::getInstance()->getVisibleSize().width + _enemy->getContentSize().width;
		}
		if ("Up" == from){
			sour.y = Director::getInstance()->getVisibleSize().height + _enemy->getContentSize().height;
		}
		if ("Down" == from){
			sour.y = -_enemy->getContentSize().height;
		}
		setEnemyPos(_map->convertToNodeSpace(sour));
		_pos = _map->convertToNodeSpace(target);
		auto move = MoveTo::create(nowData["time"].asFloat(), target);
		runAction(move);
	}
	if ("MOVO" == str){
		_aiChecked = true;
		auto pos = _map->getObjectGroup("Objects")->getObject(nowData["name"].asString());
		setEnemyPos(Vec2(pos["x"].asInt(), pos["y"].asInt() + _enemy->getContentSize().height / 2));
		for (int i = 0; i < 7; ++i)
			onDrop(0);
		auto sour = getPosition(), target = sour;
		auto to = pos["To"].asString();
		if ("Left" == to){
			target.x = -_enemy->getContentSize().width;
		}
		if ("Right" == to){
			target.x = Director::getInstance()->getVisibleSize().width + _enemy->getContentSize().width;
		}
		if ("Up" == to){
			target.y = Director::getInstance()->getVisibleSize().height + _enemy->getContentSize().height;
		}
		if ("Down" == to){
			target.y = -_enemy->getContentSize().height;
		}
		setEnemyPos(_map->convertToNodeSpace(sour));
		_pos = _map->convertToNodeSpace(target);
		auto move = MoveTo::create(nowData["time"].asFloat(), target);
		runAction(move);
	}

	if ("" != effect)
		SimpleAudioEngine::getInstance()->playEffect(effect.c_str());
	_next = nowData["next"].asFloat();
	++_id;
}

bool BossEnemy::getDamage(float damage)
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
		_eventDispatcher->dispatchCustomEvent("GamePass");
		getParent()->removeChild(this);
		return true;
	}
	return false;
}

void BossEnemy::setBallTexture(const std::string &str)
{
	_aiData = FileUtils::getInstance()->getValueMapFromFile(str);
}

void BossEnemy::setEnemyPos(const Vec2 &pos) {
	auto tar = _map->convertToWorldSpace(pos);
	if (pos.x > -_enemy->getContentSize().width - 1 && pos.x < _map->getTileSize().width*_map->getMapSize().width + _enemy->getContentSize().width + 1){
		setPosition(tar);
		_pos = pos;
	}
}
