#include "SplashScene.h"
#include "LauncherScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace CocosDenshion;

Scene* Splash::createScene()
{
    auto scene = Scene::create();
    auto layer = Splash::create();
    scene->addChild(layer);
    return scene;
}

bool Splash::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto rootNode = LayerColor::create(Color4B::WHITE);
    addChild(rootNode);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sound/0000.mp3");
	_pressNext = EventListenerTouchOneByOne::create();
	_pressNext->onTouchBegan = CC_CALLBACK_2(Splash::touchNext, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_pressNext, this);
	teamInfo();

    return true;
}

void Splash::teamInfo()
{
	_teamInfo = Label::createWithSystemFont("BIT ChangWorld Team", "Arial", 72);
	_teamInfo->setColor(Color3B::BLACK);
	_teamInfo->setOpacity(0);
	_teamInfo->setPosition(Director::getInstance()->getVisibleSize() / 2);
	auto inAct = FadeIn::create(0.5f);
	auto outAct = FadeOut::create(1.0f);
	auto delay = DelayTime::create(1.0f);
	auto act = Sequence::create(inAct, delay, outAct, CallFunc::create([this](){this->gameInfo(); }), nullptr);
	addChild(_teamInfo);
	_teamInfo->runAction(act);
}

void Splash::gameInfo()
{
	removeChild(_teamInfo);
	_gameInfo = Label::createWithSystemFont("Balala Adv", "Arial", 48);
	_gameInfo->setColor(Color3B::BLACK);
	_gameInfo->setOpacity(0);
	_gameInfo->setPosition(Director::getInstance()->getVisibleSize() / 2);
	auto inAct = FadeIn::create(0.5f);
	auto outAct = FadeOut::create(1.0f);
	auto delay = DelayTime::create(1.0f);
	auto act = Sequence::create(inAct, delay, outAct, CallFunc::create([this](){this->nextScene(); }), nullptr);
	
	_gameInfo->runAction(act);
	addChild(_gameInfo);
}

void Splash::nextScene()
{
	_eventDispatcher->removeEventListener(_pressNext);
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, Launcher::createScene()));
}

bool Splash::touchNext(Touch *touch, Event *event)
{
	nextScene();
	return false;
}
