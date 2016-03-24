#include "Scene05.h"
#include "..\Character\Player.h"
#include "..\UILinker.h"
#include "..\GameUILayer.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
//#include "FinalBoss04.h"
#include "../../CreditScene.h"
#include "../DialogLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* ChapterScene05::createScene()
{
	auto scene = Scene::create();
	auto layer = ChapterScene05::create();
	scene->addChild(layer);
	return scene;
}

bool ChapterScene05::init()
{
	if (!Layer::init()){
		return false;
	}
	
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0501.mp3", true);
	removeHideData("05");

	auto rootNode = Sprite::create("Map/Background/Chapter05-01.png");
	setBackground(rootNode, this);

	auto map = MapType::create("Map/Chapter05.tmx");
	addChild(map);

	_player = Player::create();
	addChild(_player);
	linkMapPlayer(map, _player);

	auto layer = GameUI::create();
	addChild(layer, 1);
	linkUIPlayer(layer, _player);

	linkGameOver(this);
	linkGamePass(this, CC_CALLBACK_1(ChapterScene05::gamePass, this));
	linkGameBranch(this, CC_CALLBACK_1(ChapterScene05::gameBranch, this));
	linkDialogEvent(this, "Story-05-01.dat", CC_CALLBACK_0(ChapterScene05::onDialogEnded, this));
	_flag = false;

	return true;
}

void ChapterScene05::gamePass(EventCustom *event)
{
	if (_flag)
		return;
	_flag = true;
	linkDialogEvent(this, "Story-05-02.dat", [](){
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0600.mp3", true);
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, CreditScene::createScene("CreditInfo.dat", "InfoTexture.dat")));
	});
	//Director::getInstance()->replaceScene(TransitionFade::create(0.5f, FinalBoss05::createScene()));
}

void ChapterScene05::gameBranch(EventCustom *event)
{
	pauseEnemys(_player);
	linkDialogEvent(this, "Story-05-04.dat", CC_CALLBACK_0(ChapterScene05::onDialogEnded, this));
	writeHideData("05");
}

void ChapterScene05::onDialogEnded()
{
	resumeEnemys(_player);
}

void ChapterScene05::onExitTransitionDidStart()
{
	//_eventDispatcher->removeEventListenersForTarget(this, true);
}
