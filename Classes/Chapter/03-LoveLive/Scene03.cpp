#include "Scene03.h"
#include "..\Character\Player.h"
#include "..\UILinker.h"
#include "..\GameUILayer.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "FinalBoss03.h"
#include "../DialogLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* ChapterScene03::createScene()
{
	auto scene = Scene::create();
	auto layer = ChapterScene03::create();
	scene->addChild(layer);
	return scene;
}

bool ChapterScene03::init()
{
	if (!Layer::init()){
		return false;
	}

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0301.mp3", true);
	removeHideData("03");

	auto rootNode = Sprite::create("Map/Background/Chapter03-01.png");
	setBackground(rootNode, this);

	auto map = MapType::create("Map/Chapter03.tmx");
	addChild(map);

	_player = Player::create();
	addChild(_player);
	linkMapPlayer(map, _player);

	auto layer = GameUI::create();
	addChild(layer, 1);
	layer->onSpecial = [=](){
		_player->setDamRate(0.5);
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0303.mp3", true);
		writeHideData("03", true);
	};
	linkUIPlayer(layer, _player);
	
	linkGameOver(this);
	linkGamePass(this, CC_CALLBACK_1(ChapterScene03::gamePass, this));
	linkGameBranch(this, CC_CALLBACK_1(ChapterScene03::gameBranch, this));
	linkDialogEvent(this, "Story-03-01.dat", CC_CALLBACK_0(ChapterScene03::onDialogEnded, this));

	return true;
}

void ChapterScene03::gamePass(EventCustom *event)
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, FinalBoss03::createScene()));
}

void ChapterScene03::gameBranch(EventCustom *event)
{
	pauseEnemys(_player);
	linkDialogEvent(this, "Story-03-04.dat", CC_CALLBACK_0(ChapterScene03::onDialogEnded, this));
	writeHideData("03");
}

void ChapterScene03::onDialogEnded()
{
	resumeEnemys(_player);
}

void ChapterScene03::onExitTransitionDidStart()
{
	//_eventDispatcher->removeEventListenersForTarget(this, true);
}
