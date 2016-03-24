#include "BranchEnemy.h"
#include "ui\CocosGUI.h"
#include <cmath>
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

bool BranchEnemy::init()
{
	if (!Enemy::init()){
		return false;
	}

	_health = 10;
	_maxHealth = 100;
	setHP(_health / _maxHealth*100.0f);

	_onAISel = schedule_selector(Enemy::aiAction);

	return true;
}

void BranchEnemy::aiAction(float dt)
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

bool BranchEnemy::getDamage(float damage)
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
	if (_health > _maxHealth){
		_health = _maxHealth;
		_eventDispatcher->dispatchCustomEvent("GameBranch");
		getParent()->removeChild(this);
		return true;
	}
	setHP(_health / _maxHealth * 100.0f);
	if (_health <= 0){
		SimpleAudioEngine::getInstance()->playEffect(_deadEffect.c_str());
		getParent()->removeChild(this);
		return true;
	}
	return false;
}

