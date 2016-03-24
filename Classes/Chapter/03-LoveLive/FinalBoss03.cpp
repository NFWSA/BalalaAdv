#include "FinalBoss03.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "../UILinker.h"
#include "../Character/Player.h"
#include "../ChapterSelectScene.h"
#include "../DialogLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* FinalBoss03::createScene()
{
	auto scene = Scene::create();
	auto layer = FinalBoss03::create();
	scene->addChild(layer);
	return scene;
}

void FinalBoss03::gamePass(cocos2d::EventCustom *event)
{
	gamePassSave(3);
	linkDialogEvent(this, "Story-03-03.dat", [](){
		auto scene = ChapterSelect::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
	});
}

bool FinalBoss03::init()
{
	if (!Layer::init()){
		return false;
	}

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0302.mp3", true);

	auto rootNode = Sprite::create("Map/Background/Chapter03-02.png");
	setBackground(rootNode, this);

	auto map = MapType::create("Map/Chapter03-Boss.tmx");
	addChild(map);

	_player = Player::create();
	addChild(_player);
	linkMapPlayer(map, _player);

	auto layer = GameUI::create();
	addChild(layer, 1);
	linkUIPlayer(layer, _player);
	layer->setSpecial(nullptr, existHideData("03") && !isUsedHideData("03"));
	layer->onSpecial = CC_CALLBACK_0(FinalBoss03::onSpecial, this);

	linkGameOver(this);
	linkGamePass(this, CC_CALLBACK_1(FinalBoss03::gamePass, this));
	linkDialogEvent(this, "Story-03-02.dat", CC_CALLBACK_0(FinalBoss03::onDialogEnded, this));

	if (existHideData("03") && isUsedHideData("03")){
		_player->setDamRate(0);
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0304.mp3", true);
	}

	return true;
}

void FinalBoss03::onDialogEnded()
{
	resumeEnemys(_player);
}

void FinalBoss03::onSpecial()
{
	pauseEnemys(_player);
	_player->setDamRate(0.5);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0304.mp3", true);
	linkDialogEvent(this, "Story-03-05.dat", CC_CALLBACK_0(FinalBoss03::onDialogEnded, this));
	removeHideData("03");
}

void FinalBoss03::onExitTransitionDidStart()
{
	//_eventDispatcher->removeEventListenersForTarget(this, true);
}
