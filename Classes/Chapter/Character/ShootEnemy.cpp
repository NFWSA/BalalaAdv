#include "ShootEnemy.h"
#include "ui\CocosGUI.h"
#include <cmath>
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

bool ShootEnemy::init()
{
	if (!Enemy::init()){
		return false;
	}

	_onAISel = schedule_selector(ShootEnemy::aiAction);

	return true;
}

void ShootEnemy::aiAction(float dt)
{
	setEnemyPos(_pos);
	if (_paused)
		return;

	auto hdelta = _map->convertToNodeSpace(_player->getPosition()).y - _pos.y;
	if (hdelta <= std::abs(_player->getContentSize().height - _enemy->getContentSize().height) / 2)
		onJumpBegan();
	if (_aiChecked)
		return;
	auto dis = _pos.x - _map->convertToNodeSpace(_player->getPosition()).x;
	if (Director::getInstance()->getVisibleSize().width < std::abs(dis))
		return;
	_aiChecked = true;
	std::abs(dis)>_enemy->getContentSize().width * 4 ? onShootBegan() : (dis < 0 ? toLeft(dt) : toRight(dt));
}

void ShootEnemy::setEnemyPos(const Vec2 &pos) {
	auto tar = _map->convertToWorldSpace(pos);
	if (pos.x >= 0 && pos.x <= _map->getTileSize().width*_map->getMapSize().width){
		setPosition(tar);
		_pos = pos;
	}
}

