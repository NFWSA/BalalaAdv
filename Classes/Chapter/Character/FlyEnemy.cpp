#include "FlyEnemy.h"
#include "ui\CocosGUI.h"
#include <cmath>
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

bool FlyEnemy::init()
{
	if (!Enemy::init())
		return false;

	_jumpSpeed = 16;
	_speed = 1;

	_onJumpSel = schedule_selector(FlyEnemy::onJump);
	_onFallSel = schedule_selector(FlyEnemy::onFall);
	_onDropSel = schedule_selector(FlyEnemy::onDrop);
	_onAISel = schedule_selector(FlyEnemy::aiAction);

	return true;
}

void FlyEnemy::aiAction(float dt)
{
	setEnemyPos(_pos);
	if (_paused)
		return;

	if (_aiChecked)
		return;
	auto dis = _pos.x - _map->convertToNodeSpace(_player->getPosition()).x;
	auto hdelta = _pos.y -  _map->convertToNodeSpace(_player->getPosition()).y;
	if (Director::getInstance()->getVisibleSize().width < std::abs(dis))
		return;
	_aiChecked = true;
	dis > 0 ? toLeft(dt) : toRight(dt);
	if (std::abs(dis)<_enemy->getContentSize().width && hdelta > _enemy->getContentSize().height)
		onJumpBegan();
}

void FlyEnemy::onJump(float dt)
{
	for (int i = _jumpSpeed; i > 0; --i){
		Vec2 tar(_pos.x, _pos.y - i);
		if (isTarDisenabled(tar)){
			continue;
		}
		setEnemyPos(tar);
		return;
	}
	unschedule(_onJumpSel);
	schedule(_onFallSel, 0.0125f);
}

void FlyEnemy::onFall(float dt)
{
	for (int i = _jumpSpeed; i > 0; --i){
		Vec2 tar(_pos.x, _pos.y + i);
		if (isTarDisenabled(tar) || tar.y > _map->getMapSize().height*_map->getTileSize().height - getContentSize().height / 2)
			continue;
		setEnemyPos(tar);
		return;
	}
	_enemyJumped = false;
	unschedule(_onFallSel);
}

void FlyEnemy::onDrop(float dt)
{
	for (int i = _jumpSpeed; i > 0; --i){
		Vec2 tar(_pos.x, _pos.y + i);
		if (isTarDisenabled(tar) || tar.y > _map->getMapSize().height*_map->getTileSize().height - getContentSize().height / 2)
			continue;
		setEnemyPos(tar);
		return;
	}
	unschedule(_onDropSel);
}
