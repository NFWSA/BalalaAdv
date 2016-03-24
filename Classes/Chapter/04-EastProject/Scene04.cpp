#include "Scene04.h"
#include "..\Character\Player.h"
#include "..\UILinker.h"
#include "..\GameUILayer.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "FinalBoss04.h"
#include "../DialogLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* ChapterScene04::createScene()
{
	auto scene = Scene::create();
	auto layer = ChapterScene04::create();
	scene->addChild(layer);
	return scene;
}

bool ChapterScene04::init()
{
	if (!Layer::init()){
		return false;
	}

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0401.mp3", true);
	removeHideData("04");

	auto rootNode = Sprite::create("Map/Background/Chapter04-01.png");
	setBackground(rootNode, this);

	auto map = MapType::create("Map/Chapter04.tmx");
	addChild(map);

	_player = Player::create();
	addChild(_player);
	linkMapPlayer(map, _player);

	auto layer = GameUI::create();
	addChild(layer, 1);
	layer->onSpecial = [=](){pauseEnemys(_player); removeHideData("04"); };
	linkUIPlayer(layer, _player);

	linkGameOver(this);
	linkGamePass(this, CC_CALLBACK_1(ChapterScene04::gamePass, this));
	linkGameBranch(this, CC_CALLBACK_1(ChapterScene04::gameBranch, this));
	linkDialogEvent(this, "Story-04-01.dat", CC_CALLBACK_0(ChapterScene04::onDialogEnded, this));

	return true;
}

void ChapterScene04::gamePass(EventCustom *event)
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, FinalBoss04::createScene()));
}

void ChapterScene04::gameBranch(EventCustom *event)
{
	pauseEnemys(_player);
	linkDialogEvent(this, "Story-04-04.dat", CC_CALLBACK_0(ChapterScene04::onDialogEnded, this));
	writeHideData("04");
}

void ChapterScene04::onDialogEnded()
{
	resumeEnemys(_player);
}

void ChapterScene04::onExitTransitionDidStart()
{
	//_eventDispatcher->removeEventListenersForTarget(this, true);
}
