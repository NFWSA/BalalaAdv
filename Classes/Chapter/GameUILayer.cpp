#include "GameUILayer.h"
#include "ui/CocosGUI.h"
#include <cmath>
#include "../HelpScene.h"

USING_NS_CC;

bool GameUI::init()
{
	if (!Layer::init()){
		return false;
	}

	onUpBegan = onDownBegan = onLeftBegan = onRightBegan = onJumpBegan = nullptr;
	onUpEnded = onDownEnded = onLeftEnded = onRightEnded = nullptr;
	onJumpEnded = onSpecial = nullptr;
	onMode1 = onMode2 = onMode3 = nullptr;
	onClickMoved = onClickEnded = nullptr;
	_specialBtn = nullptr;

	auto cc = Sprite::create("UI/HPBar.png");
	_hpBar = ProgressTimer::create(cc);
	_hpBar->setType(ProgressTimer::Type::BAR);
	_hpBar->setPercentage(100);
	_hpBar->setMidpoint(Vec2(0, 0.5f));
	_hpBar->setBarChangeRate(Vec2(1, 0));
	_hpBar->setPosition(Vec2(10 + _hpBar->getContentSize().width / 2, Director::getInstance()->getVisibleSize().height - 10 - _hpBar->getContentSize().height / 2));
	addChild(_hpBar);
	_hpBox = Sprite::create("UI/HPBox.png");
	addChild(_hpBox);
	_hpBox->setPosition(Vec2(10 + _hpBox->getContentSize().width / 2, Director::getInstance()->getVisibleSize().height - 10 - _hpBox->getContentSize().height / 2));
	auto proToAct = ProgressFromTo::create(0.2f, 0.0f, 100.0f);
	_hpBar->runAction(proToAct);

	_pan = Sprite::create("UI/Pan.png");
	_pan->setScale(Director::getInstance()->getVisibleSize().height / 3 / _pan->getContentSize().height);
	_pan->setPosition(_pan->getContentSize() / 2 * _pan->getScale());
	_pan->setOpacity(120);
	addChild(_pan);
	_movePan = Sprite::create("UI/MovePan.png");
	_movePan->setScale(_pan->getContentSize().height*_pan->getScale() / 3 / _movePan->getContentSize().height);
	_movePan->setPosition(_pan->getPosition());
	addChild(_movePan);

	_mode1Check = ui::Button::create("UI/Mode1_Unchecked.png", "", "UI/Mode1_Checked.png");
	_mode1Check->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - (_mode1Check->getContentSize().width * 5 / 2) - 20,
														Director::getInstance()->getVisibleSize().height - (_mode1Check->getContentSize().height / 2)));
	addChild(_mode1Check);
	_mode1Check->addClickEventListener([=](Ref *ref){
		if (nullptr != onMode1){
			if (onMode1()){
				_mode1Check->setBright(false);
				_mode1Check->setEnabled(false);
				_mode2Check->setBright(true);
				_mode2Check->setEnabled(true);
				_mode3Check->setBright(true);
				_mode3Check->setEnabled(true);
			}
		}
	});
	_mode1Check->setBright(false);

	_mode2Check = ui::Button::create("UI/Mode2_Unchecked.png", "", "UI/Mode2_Checked.png");
	_mode2Check->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - (_mode1Check->getContentSize().width * 3 / 2) - 10,
														Director::getInstance()->getVisibleSize().height - (_mode1Check->getContentSize().height / 2)));
	addChild(_mode2Check);
	_mode2Check->addClickEventListener([=](Ref *ref){
		if (nullptr != onMode2){
			if (onMode2()){
				_mode1Check->setBright(true);
				_mode1Check->setEnabled(true);
				_mode2Check->setBright(false);
				_mode2Check->setEnabled(false);
				_mode3Check->setBright(true);
				_mode3Check->setEnabled(true);
			}
		}
	});

	_mode3Check = ui::Button::create("UI/Mode3_Unchecked.png", "", "UI/Mode3_Checked.png");
	_mode3Check->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - (_mode1Check->getContentSize().width / 2),
														Director::getInstance()->getVisibleSize().height - (_mode1Check->getContentSize().height / 2)));
	addChild(_mode3Check);
	_mode3Check->addClickEventListener([=](Ref *ref){
		if (nullptr != onMode3){
			if (onMode3()){
				_mode1Check->setBright(true);
				_mode1Check->setEnabled(true);
				_mode2Check->setBright(true);
				_mode2Check->setEnabled(true);
				_mode3Check->setBright(false);
				_mode3Check->setEnabled(false);
			}
		}
	});

	_jumpBtn = ui::Button::create("UI/Jump.png", "UI/Jump_Click.png");
	addChild(_jumpBtn);
	_jumpBtn->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - _jumpBtn->getContentSize().width / 2 - 10, _jumpBtn->getContentSize().height / 2 + 10));
	_jumpBtn->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
		if (ui::Widget::TouchEventType::BEGAN == type){
			if (nullptr != onJumpBegan)
				this->onJumpBegan();
			return;
		}
		if (ui::Widget::TouchEventType::ENDED == type){
			if (nullptr != onJumpEnded)
				this->onJumpEnded();
			return;
		}
	});

	_pauseBtn = ui::Button::create("UI/Pause.png", "UI/Pause_Click.png");
	addChild(_pauseBtn);
	_pauseBtn->setPosition(Vec2(10 + _pauseBtn->getContentSize().width / 2, _hpBox->getPositionY() - _hpBox->getContentSize().height / 2 - 10 - _pauseBtn->getContentSize().height / 2));
	_pauseBtn->addClickEventListener([=](Ref *ref){
		Director::getInstance()->pushScene(HelpUI::createScene());
	});

	_specialBtn = ui::Button::create("UI/Special.png", "UI/Special_Click.png");
	addChild(_specialBtn);
	_specialBtn->setPosition(Vec2(_jumpBtn->getPositionX() - 10 - (_jumpBtn->getContentSize().width + _specialBtn->getContentSize().width) / 2, _jumpBtn->getPositionY()));
	_specialBtn->addClickEventListener([=](Ref *ref){
		if (nullptr != onSpecial)
			this->onSpecial();
		_specialBtn->setVisible(false);
	});
	_specialBtn->setVisible(false);

	_keyListener = EventListenerKeyboard::create();
	_keyListener->onKeyPressed = CC_CALLBACK_2(GameUI::keyPressedEvent, this);
	_keyListener->onKeyReleased = CC_CALLBACK_2(GameUI::keyReleasedEvent, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_keyListener, this);

	_panListener = EventListenerTouchOneByOne::create();
	_panListener->onTouchBegan = CC_CALLBACK_2(GameUI::panMoveBegan, this);
	_panListener->onTouchMoved = CC_CALLBACK_2(GameUI::panMoved, this);
	_panListener->onTouchEnded = CC_CALLBACK_2(GameUI::panMoveEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_panListener, _movePan);
	_screenListener = EventListenerTouchOneByOne::create();
	_screenListener->onTouchBegan = CC_CALLBACK_2(GameUI::screenTouchBegan, this);
	_screenListener->onTouchMoved = CC_CALLBACK_2(GameUI::screenTouchMoved, this);
	_screenListener->onTouchEnded = CC_CALLBACK_2(GameUI::screenTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_screenListener, this);

	_branchListener = EventListenerCustom::create("GameBranch", CC_CALLBACK_1(GameUI::setSpecial, this, true));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_branchListener, this);

	return true;
}

bool GameUI::panMoveBegan(Touch* touch, Event* event)
{
	auto dis = _pan->getPosition().distance(touch->getLocation());
	if (dis > _movePan->getContentSize().width*_movePan->getScale())
		return false;
	panMoved(touch, event);
	return true;
}

void GameUI::panMoved(Touch* touch, Event* event)
{
	auto target = touch->getLocation();
	auto dis = _pan->getPosition().distance(target);
	auto nor = (target - _pan->getPosition()).getNormalized();
	if (dis > _movePan->getContentSize().width*_movePan->getScale())
		dis = _movePan->getContentSize().width*_movePan->getScale();
	_movePan->setPosition(_pan->getPosition() + nor*dis);

	int nowFront = 0;
	auto angle = nor.getAngle()*180.0 / std::_Pi;
	if (angle >= -60.0&&angle <= 60.0){
		nowFront |= RIGHT;
		if (nullptr != onRightBegan&&!(_front&RIGHT)){
			onRightBegan();
		}
	}
	if (angle >= 30.0&&angle <= 150.0){
		nowFront |= UP;
		if (nullptr != onUpBegan&&!(_front&UP)){
			onUpBegan();
		}
	}
	if (angle >= -150.0&&angle <= -30.0){
		nowFront |= DOWN;
		if (nullptr != onDownBegan&&!(_front&DOWN)){
			onDownBegan();
		}
	}
	if (angle >= 120.0 || angle <= -120.0){
		nowFront |= LEFT;
		if (nullptr != onLeftBegan&&!(_front&LEFT)){
			onLeftBegan();
		}
	}

	if ((_front&RIGHT) && !(nowFront&RIGHT)){
		if (nullptr != onRightEnded){
			onRightEnded();
		}
	}
	if ((_front&UP) && !(nowFront&UP)){
		if (nullptr != onUpEnded){
			onUpEnded();
		}
	}
	if ((_front&LEFT) && !(nowFront&LEFT)){
		if (nullptr != onLeftEnded){
			onLeftEnded();
		}
	}
	if ((_front&DOWN) && !(nowFront&DOWN)){
		if (nullptr != onDownEnded){
			onDownEnded();
		}
	}
	_front = nowFront;
}

void GameUI::panMoveEnded(Touch* touch, Event* event)
{
	_movePan->setPosition(_pan->getPosition());
	if ((_front&UP) && (nullptr != onUpEnded))
		onUpEnded();
	if ((_front&DOWN) && (nullptr != onDownEnded))
		onDownEnded();
	if ((_front&LEFT) && (nullptr != onLeftEnded))
		onLeftEnded();
	if ((_front&RIGHT) && (nullptr != onRightEnded))
		onRightEnded();
	_front = 0;
}

bool GameUI::screenTouchBegan(Touch* touch, Event* event)
{
	screenTouchMoved(touch, event);
	return true;
}

void GameUI::screenTouchMoved(Touch* touch, Event* event)
{
	if (nullptr != onClickMoved)
		onClickMoved(touch, event);
}

void GameUI::screenTouchEnded(Touch* touch, Event* event)
{
	if (nullptr != onClickEnded)
		onClickEnded(touch, event);
}

void GameUI::keyPressedEvent(EventKeyboard::KeyCode key, Event* event)
{
	if (EventKeyboard::KeyCode::KEY_W == key || EventKeyboard::KeyCode::KEY_UP_ARROW == key){
		if (nullptr != onUpBegan)
			onUpBegan();
		return;
	}
	if (EventKeyboard::KeyCode::KEY_S == key || EventKeyboard::KeyCode::KEY_DOWN_ARROW == key){
		if (nullptr != onDownBegan)
			onDownBegan();
		return;
	}
	if (EventKeyboard::KeyCode::KEY_A == key || EventKeyboard::KeyCode::KEY_LEFT_ARROW == key){
		if (nullptr != onLeftBegan)
			onLeftBegan();
		return;
	}
	if (EventKeyboard::KeyCode::KEY_D == key || EventKeyboard::KeyCode::KEY_RIGHT_ARROW == key){
		if (nullptr != onRightBegan)
			onRightBegan();
		return;
	}

	if (EventKeyboard::KeyCode::KEY_SPACE == key){
		if (nullptr != onJumpBegan)
			onJumpBegan();
		return;
	}
}

void GameUI::keyReleasedEvent(EventKeyboard::KeyCode key, Event* event)
{
	if (EventKeyboard::KeyCode::KEY_W == key || EventKeyboard::KeyCode::KEY_UP_ARROW == key){
		if (nullptr != onUpEnded)
			onUpEnded();
		return;
	}
	if (EventKeyboard::KeyCode::KEY_S == key || EventKeyboard::KeyCode::KEY_DOWN_ARROW == key){
		if (nullptr != onDownEnded)
			onDownEnded();
		return;
	}
	if (EventKeyboard::KeyCode::KEY_A == key || EventKeyboard::KeyCode::KEY_LEFT_ARROW == key){
		if (nullptr != onLeftEnded)
			onLeftEnded();
		return;
	}
	if (EventKeyboard::KeyCode::KEY_D == key || EventKeyboard::KeyCode::KEY_RIGHT_ARROW == key){
		if (nullptr != onRightEnded)
			onRightEnded();
		return;
	}

	if (EventKeyboard::KeyCode::KEY_SPACE == key){
		if (nullptr != onJumpEnded)
			onJumpEnded();
		return;
	}
	if (EventKeyboard::KeyCode::KEY_1 == key){
		if (nullptr != onMode1){
			if (onMode1()){
				_mode1Check->setBright(false);
				_mode1Check->setEnabled(false);
				_mode2Check->setBright(true);
				_mode2Check->setEnabled(true);
				_mode3Check->setBright(true);
				_mode3Check->setEnabled(true);
			}
		}
		return;
	}
	if (EventKeyboard::KeyCode::KEY_2 == key){
		if (nullptr != onMode2){
			if (onMode2()){
				_mode1Check->setBright(true);
				_mode1Check->setEnabled(true);
				_mode2Check->setBright(false);
				_mode2Check->setEnabled(false);
				_mode3Check->setBright(true);
				_mode3Check->setEnabled(true);
			}
		}
		return;
	}
	if (EventKeyboard::KeyCode::KEY_3 == key){
		if (nullptr != onMode3){
			if (onMode3()){
				_mode1Check->setBright(true);
				_mode1Check->setEnabled(true);
				_mode2Check->setBright(true);
				_mode2Check->setEnabled(true);
				_mode3Check->setBright(false);
				_mode3Check->setEnabled(false);
			}
		}
		return;
	}

	if (EventKeyboard::KeyCode::KEY_LEFT_CTRL == key){
		if (_specialBtn->isVisible() && nullptr != onSpecial)
			onSpecial();
		_specialBtn->setVisible(false);
		return;
	}
	if (EventKeyboard::KeyCode::KEY_ESCAPE == key){
		Director::getInstance()->pushScene(HelpUI::createScene());
		return;
	}

}

void GameUI::setSpecial(cocos2d::EventCustom *event, bool flag)
{
	if (nullptr != _specialBtn)
		_specialBtn->setVisible(flag);
}

void GameUI::setHP(float dt)
{
	auto dam = ProgressTo::create(0.5f, dt);
	_hpBar->runAction(dam);
}