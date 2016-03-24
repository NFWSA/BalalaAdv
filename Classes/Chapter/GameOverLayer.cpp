#include "cocostudio/CocoStudio.h"
#include "UILinker.h"
#include "GameOverLayer.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "../LauncherScene.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocostudio::timeline;

void GameOver::onEnterTransitionDidFinish()
{
	_next->addClickEventListener(CC_CALLBACK_1(GameOver::onClick, this));
}

bool GameOver::init()
{
	if (!Layer::init())
		return false;

	auto layer = LayerColor::create(Color4B(77, 77, 77, 200));
	addChild(layer);

	onRestart = nullptr;
	auto info = Label::createWithSystemFont("Game Over", "Arial", 64);
	addChild(info);
	info->setPosition(Director::getInstance()->getVisibleSize() / 2);
	_next = ui::Button::create();
	_next->setTitleText("Please touch here to restart...");
	_next->setTitleFontName("Arial");
	_next->setTitleFontSize(36);
	addChild(_next);
	_next->setPosition(Vec2(info->getPositionX(), info->getPositionY() - info->getContentSize().height));

	return true;
}

void GameOver::onClick(Ref *ref)
{
	Director::getInstance()->popScene();
}
