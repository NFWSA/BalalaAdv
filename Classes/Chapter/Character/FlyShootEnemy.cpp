#include "FlyShootEnemy.h"
#include "ui\CocosGUI.h"
#include <cmath>
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

bool FlyShootEnemy::init()
{
	if (!Enemy::init())
		return false;

	_jumpSpeed = 16;
	_speed = 1;

	_onDropSel = schedule_selector(FlyShootEnemy::onDrop);
	_onAISel = schedule_selector(FlyShootEnemy::aiAction);

	return true;
}

void FlyShootEnemy::aiAction(float dt)
{
	setEnemyPos(_pos);
	if (_paused)
		return;

	auto dis = _pos.x - _map->convertToNodeSpace(_player->getPosition()).x;
	if (Director::getInstance()->getVisibleSize().width < std::abs(dis))
		return;
	onShootBegan();
	if (_aiChecked)
		return;
	auto hdelta = _pos.y -  _map->convertToNodeSpace(_player->getPosition()).y;
	dis < 0 ? toLeft(dt) : toRight(dt);
}

void FlyShootEnemy::onDrop(float dt)
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

void FlyShootEnemy::setEnemyPos(const Vec2 &pos) {
	auto tar = _map->convertToWorldSpace(pos);
	if (pos.x >= 0 && pos.x <= _map->getTileSize().width*_map->getMapSize().width){
		setPosition(tar);
		_pos = pos;
	}
}
