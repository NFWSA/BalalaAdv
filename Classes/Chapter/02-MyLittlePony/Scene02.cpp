#include "Scene02.h"
#include "..\Character\Player.h"
#include "..\UILinker.h"
#include "..\GameUILayer.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "FinalBoss02.h"
#include "../DialogLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* ChapterScene02::createScene()
{
	auto scene = Scene::create();
	auto layer = ChapterScene02::create();
	scene->addChild(layer);
	return scene;
}

bool ChapterScene02::init()
{
	if (!Layer::init()){
		return false;
	}

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0201.mp3", true);
	removeHideData("02");

	auto rootNode = Sprite::create("Map/Background/Chapter02-01.png");
	setBackground(rootNode, this);

	auto map = MapType::create("Map/Chapter02.tmx");
	addChild(map);

	_player = Player::create();
	addChild(_player);
	linkMapPlayer(map, _player);

	auto layer = GameUI::create();
	addChild(layer, 1);
	layer->onSpecial = [=](){_player->setDamRate(-0.3); removeHideData("02"); };
	linkUIPlayer(layer, _player);

	linkGameOver(this);
	linkGamePass(this, CC_CALLBACK_1(ChapterScene02::gamePass, this));
	linkGameBranch(this, CC_CALLBACK_1(ChapterScene02::gameBranch, this));
	linkDialogEvent(this, "Story-02-01.dat", CC_CALLBACK_0(ChapterScene02::onDialogEnded, this));

	return true;
}

void ChapterScene02::gamePass(EventCustom *event)
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, FinalBoss02::createScene()));
}

void ChapterScene02::gameBranch(EventCustom *event)
{
	pauseEnemys(_player);
	linkDialogEvent(this, "Story-02-04.dat", CC_CALLBACK_0(ChapterScene02::onDialogEnded, this));
	writeHideData("02");
}

void ChapterScene02::onDialogEnded()
{
	resumeEnemys(_player);
}

void ChapterScene02::onExitTransitionDidStart()
{
	//_eventDispatcher->removeEventListenersForTarget(this, true);
}
