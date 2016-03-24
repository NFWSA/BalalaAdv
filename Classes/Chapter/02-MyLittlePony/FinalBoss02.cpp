#include "FinalBoss02.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "../UILinker.h"
#include "../Character/Player.h"
#include "../ChapterSelectScene.h"
#include "../DialogLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* FinalBoss02::createScene()
{
	auto scene = Scene::create();
	auto layer = FinalBoss02::create();
	scene->addChild(layer);
	return scene;
}

void FinalBoss02::gamePass(cocos2d::EventCustom *event)
{
	gamePassSave(2);
	linkDialogEvent(this, "Story-02-03.dat", [](){
		auto scene = ChapterSelect::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
	});
}

bool FinalBoss02::init()
{
	if (!Layer::init()){
		return false;
	}

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0202.mp3", true);

	auto rootNode = Sprite::create("Map/Background/Chapter02-02.png");
	setBackground(rootNode, this);

	auto map = MapType::create("Map/Chapter02-Boss.tmx");
	addChild(map);

	_player = Player::create();
	addChild(_player);
	linkMapPlayer(map, _player);

	auto layer = GameUI::create();
	addChild(layer, 1);
	linkUIPlayer(layer, _player);
	layer->setSpecial(nullptr, existHideData("02"));
	layer->onSpecial = CC_CALLBACK_0(FinalBoss02::onSpecial, this);

	linkGameOver(this);
	linkGamePass(this, CC_CALLBACK_1(FinalBoss02::gamePass, this));
	linkDialogEvent(this, "Story-02-02.dat", CC_CALLBACK_0(FinalBoss02::onDialogEnded, this));

	return true;
}

void FinalBoss02::onDialogEnded()
{
	resumeEnemys(_player);
}

void FinalBoss02::onSpecial()
{
	pauseEnemys(_player);
	linkDialogEvent(this, "Story-02-05.dat", nullptr);
	removeHideData("02");
}

void FinalBoss02::onExitTransitionDidStart()
{
	//_eventDispatcher->removeEventListenersForTarget(this, true);
}
