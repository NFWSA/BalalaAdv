#include "Scene01.h"
#include "..\Character\Player.h"
#include "..\UILinker.h"
#include "..\GameUILayer.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "FinalBoss01.h"
#include "../DialogLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* ChapterScene01::createScene()
{
	auto scene = Scene::create();
	auto layer = ChapterScene01::create();
	scene->addChild(layer);
	return scene;
}

bool ChapterScene01::init()
{
	if (!Layer::init()){
		return false;
	}

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0101.mp3", true);
	removeHideData("01");

	auto rootNode = Sprite::create("Map/Background/Chapter01-01.png");
	setBackground(rootNode, this);

	auto map = MapType::create("Map/Chapter01.tmx");
	addChild(map);

	_player = Player::create();
	addChild(_player);
	linkMapPlayer(map, _player);

	auto layer = GameUI::create();
	addChild(layer, 1);
	layer->onSpecial = [=](){damageAllEnemys(_player, 590); removeHideData("01"); };
	linkUIPlayer(layer, _player);

	linkGameOver(this);
	linkGamePass(this, CC_CALLBACK_1(ChapterScene01::gamePass, this));
	linkGameBranch(this, CC_CALLBACK_1(ChapterScene01::gameBranch, this));
	linkDialogEvent(this, "Story-01-01.dat", CC_CALLBACK_0(ChapterScene01::onDialogEnded, this));

	return true;
}

void ChapterScene01::gamePass(EventCustom *event)
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, FinalBoss01::createScene()));
}

void ChapterScene01::gameBranch(EventCustom *event)
{
	pauseEnemys(_player);
	linkDialogEvent(this, "Story-01-04.dat", CC_CALLBACK_0(ChapterScene01::onDialogEnded, this));
	writeHideData("01");
}

void ChapterScene01::onDialogEnded()
{
	resumeEnemys(_player);
}

void ChapterScene01::onExitTransitionDidStart()
{
	//_eventDispatcher->removeEventListenersForTarget(this, false);
}
