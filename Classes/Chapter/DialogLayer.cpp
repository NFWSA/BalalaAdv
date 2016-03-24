#include "DialogLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <cmath>

USING_NS_CC;

using namespace cocostudio::timeline;

DialogLayer* DialogLayer::createWithFile(const std::string &str)
{
	DialogLayer *pRet = new(std::nothrow) DialogLayer();
	if (pRet&&pRet->init()){
		pRet->autorelease();
		pRet->setDataFile(str);
		return pRet;
	}
	else{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void DialogLayer::setDataFile(const std::string &str)
{
	_data = FileUtils::getInstance()->getValueMapFromFile(str);
	_this->setTexture(_data["character" + Value(_id).asString()].asValueMap()["Texture"].asString());
	_thisSpeech->setString(_data["character" + Value(_id).asString()].asValueMap()["sentence" + Value(_num).asString()].asString());
}

bool DialogLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	_thisBox = Sprite::create("UI/ThisSpeech.png");
	_thatBox = Sprite::create("UI/ThatSpeech.png");
	addChild(_thisBox);
	addChild(_thatBox);
	_thisBox->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 10 + _thisBox->getContentSize().height / 2));
	_thatBox->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 50 + _thatBox->getContentSize().height / 2 * 3));

	_this = Sprite::create();
	_that = Sprite::create();
	addChild(_this);
	addChild(_that);
	auto size = _thisBox->getContentSize();
	size.height -= 20;
	size.width = 96;
	_this->setContentSize(size);
	_that->setContentSize(size);
	_this->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 20 - _this->getContentSize().width / 2, 20 + _this->getContentSize().height / 2));
	_that->setPosition(Vec2(20 + _that->getContentSize().width / 2, Director::getInstance()->getVisibleSize().height - 20 - _that->getContentSize().height / 2));

	_thisSpeech = Label::createWithSystemFont("", "Arial", 32);
	_thatSpeech = Label::createWithSystemFont("", "Arial", 32);
	addChild(_thisSpeech);
	addChild(_thatSpeech);
	_thisSpeech->setDimensions(_thisBox->getContentSize().width - 30 - _this->getContentSize().width, _thisBox->getContentSize().height - 40);
	_thisSpeech->setPosition(Vec2(_thisBox->getPositionX() - _this->getContentSize().width / 2 - 5, _thisBox->getPositionY()));
	_thatSpeech->setDimensions(_thatBox->getContentSize().width - 30 - _that->getContentSize().width, _thatBox->getContentSize().height - 40);
	_thatSpeech->setPosition(Vec2(_thatBox->getPositionX() + _that->getContentSize().width / 2 + 5, _thatBox->getPositionY()));

	_touchEvent = EventListenerTouchOneByOne::create();
	_touchEvent->onTouchBegan = CC_CALLBACK_2(DialogLayer::onTouchBegan, this);
	_touchEvent->onTouchEnded = CC_CALLBACK_2(DialogLayer::onTouchEnded, this);
	_touchEvent->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchEvent, this);

	_id = _num = 0;
	onEnded = nullptr;

    return true;
}

void DialogLayer::onExitTransitionDidStart()
{
	_eventDispatcher->removeEventListener(_touchEvent);
}

void DialogLayer::onExit()
{
	_eventDispatcher->removeEventListener(_touchEvent);
}

bool DialogLayer::onTouchBegan(cocos2d::Touch*, cocos2d::Event*)
{
	return true;
}

void DialogLayer::onTouchEnded(cocos2d::Touch*, cocos2d::Event*)
{
	 ++_num;
	 if (_data["character" + Value(_id).asString()].asValueMap()["number"].asInt() == _num){
		 ++_id;
		 _num = 0;
		 if (_data["number"].asInt() == _id){
			 if (nullptr != onEnded)
				 onEnded();
			 getParent()->removeChild(this);
			 return;
		 }
	 }
	 auto tex = _data["character" + Value(_id).asString()].asValueMap()["Texture"].asString();
	 auto sent = _data["character" + Value(_id).asString()].asValueMap()["sentence" + Value(_num).asString()].asString();
	 if (_id % 2 == 0){
		 _this->setTexture(tex);
		 _thisSpeech->setString(sent);
	 }
	 else{
		 _that->setTexture(tex);
		 _thatSpeech->setString(sent);
	 }
}
