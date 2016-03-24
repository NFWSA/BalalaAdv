#include "FinalBoss01.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "../UILinker.h"
#include "../Character/Player.h"
#include "../ChapterSelectScene.h"
#include "../DialogLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* FinalBoss01::createScene()
{
	auto scene = Scene::create();
	auto layer = FinalBoss01::create();
	scene->addChild(layer);
	return scene;
}

void FinalBoss01::gamePass(cocos2d::EventCustom *event)
{
	gamePassSave(1);
	linkDialogEvent(this, "Story-01-03.dat", [](){
		auto scene = ChapterSelect::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
	});
}

bool FinalBoss01::init()
{
	if (!Layer::init()){
		return false;
	}
	
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0102.mp3", true);
	
	auto rootNode = Sprite::create("Map/Background/Chapter01-02.png");
	setBackground(rootNode, this);

	auto map = MapType::create("Map/Chapter01-Boss.tmx");
	addChild(map);

	_player = Player::create();
	addChild(_player);
	linkMapPlayer(map, _player);

	auto layer = GameUI::create();
	addChild(layer, 1);
	linkUIPlayer(layer, _player);
	layer->setSpecial(nullptr, existHideData("01"));
	layer->onSpecial = CC_CALLBACK_0(FinalBoss01::onSpecial, this);

	linkGameOver(this);
	linkGamePass(this, CC_CALLBACK_1(FinalBoss01::gamePass, this));
	linkDialogEvent(this, "Story-01-02.dat", CC_CALLBACK_0(FinalBoss01::onDialogEnded, this));
	
	return true;
}

void FinalBoss01::onDialogEnded()
{
	resumeEnemys(_player);
}

void FinalBoss01::onSpecial()
{
	pauseEnemys(_player);
	linkDialogEvent(this, "Story-01-05.dat", CC_CALLBACK_0(FinalBoss01::onDialogEnded, this));
	damageAllEnemys(_player, 590); 
	removeHideData("01");
}

void FinalBoss01::onExitTransitionDidStart()
{
	//_eventDispatcher->removeEventListenersForTarget(this, true);
}
