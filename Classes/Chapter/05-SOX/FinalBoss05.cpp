#include "FinalBoss04.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "../UILinker.h"
#include "../Character/Player.h"
#include "../ChapterSelectScene.h"
#include "../DialogLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* FinalBoss04::createScene()
{
	auto scene = Scene::create();
	auto layer = FinalBoss04::create();
	scene->addChild(layer);
	return scene;
}

void FinalBoss04::gamePass(cocos2d::EventCustom *event)
{
	gamePassSave(3);
	linkDialogEvent(this, "Story-04-03.dat", [](){
		auto scene = ChapterSelect::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
	});
}

bool FinalBoss04::init()
{
	if (!Layer::init()){
		return false;
	}

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0402.mp3", true);

	auto rootNode = Sprite::create("Map/Background/Chapter04-02.png");
	setBackground(rootNode, this);

	auto map = MapType::create("Map/Chapter04-Boss.tmx");
	addChild(map);

	_player = Player::create();
	addChild(_player);
	linkMapPlayer(map, _player);

	auto layer = GameUI::create();
	addChild(layer, 1);
	linkUIPlayer(layer, _player);
	layer->setSpecial(nullptr, existHideData("04"));
	layer->onSpecial = CC_CALLBACK_0(FinalBoss04::onSpecial, this);

	linkGameOver(this);
	linkGamePass(this, CC_CALLBACK_1(FinalBoss04::gamePass, this));
	linkDialogEvent(this, "Story-04-02.dat", CC_CALLBACK_0(FinalBoss04::onDialogEnded, this));

	return true;
}

void FinalBoss04::onDialogEnded()
{
	resumeEnemys(_player);
}

void FinalBoss04::onSpecial()
{
	pauseEnemys(_player);
	linkDialogEvent(this, "Story-04-05.dat", nullptr);
	removeHideData("04");
}

void FinalBoss04::onExitTransitionDidStart()
{
	//_eventDispatcher->removeEventListenersForTarget(this, true);
}
