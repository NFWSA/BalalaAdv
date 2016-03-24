#include "LauncherScene.h"
#include "SaveSelectScene.h"
#include "HelpScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace CocosDenshion;

Scene* Launcher::createScene()
{
    auto scene = Scene::create();
    auto layer = Launcher::create();
    scene->addChild(layer);
    return scene;
}

void Launcher::onEnterTransitionDidFinish()
{
	auto fileUtils = FileUtils::getInstance();
	if (fileUtils->isFileExist("Save/0.sav") || fileUtils->isFileExist("Save/1.sav") || fileUtils->isFileExist("Save/2.sav"))
		_loadBtn->setEnabled(true);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0000.mp3", true);
}

bool Launcher::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto rootNode = LayerColor::create(Color4B::BLUE);
    addChild(rootNode);

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/0000.mp3", true);

	auto background = Sprite::create("Launcher.png");
	background->setPosition(Director::getInstance()->getVisibleSize() / 2);
	background->setScale(Director::getInstance()->getVisibleSize().width / background->getContentSize().width);
	addChild(background);

	auto layout = ui::VBox::create();
	addChild(layout);
	auto startBtn = ui::Button::create();
	startBtn->setTitleText("Start");
	startBtn->setTitleFontName("Cooper Std");
	startBtn->setTitleFontSize(36);
	startBtn->setColor(Color3B::BLACK);
	startBtn->addClickEventListener(CC_CALLBACK_0(Launcher::startGame, this));
	layout->addChild(startBtn);
	_loadBtn = ui::Button::create();
	_loadBtn->setTitleText("Load Game");
	_loadBtn->setTitleFontName("Cooper Std");
	_loadBtn->setTitleFontSize(36);
	_loadBtn->setColor(Color3B::BLACK);
	_loadBtn->addClickEventListener(CC_CALLBACK_0(Launcher::loadGame, this));
	layout->addChild(_loadBtn);
	auto fileUtils = FileUtils::getInstance();
	if (!(fileUtils->isFileExist("Save/0.sav") || fileUtils->isFileExist("Save/1.sav") || fileUtils->isFileExist("Save/2.sav")))
		_loadBtn->setEnabled(false);
	auto settingBtn = ui::Button::create();
	settingBtn->setTitleText("Help");
	settingBtn->setTitleFontName("Cooper Std");
	settingBtn->setTitleFontSize(36);
	settingBtn->setColor(Color3B::BLACK);
	settingBtn->addClickEventListener(CC_CALLBACK_0(Launcher::settingGame, this));
	layout->addChild(settingBtn);
	auto exitBtn = ui::Button::create();
	exitBtn->setTitleText("Exit");
	exitBtn->setTitleFontName("Cooper Std");
	exitBtn->setTitleFontSize(36);
	exitBtn->setColor(Color3B::BLACK);
	exitBtn->addClickEventListener(CC_CALLBACK_0(Launcher::exitGame, this));
	layout->addChild(exitBtn);
	layout->setPosition(Vec2((Director::getInstance()->getVisibleSize().width - _loadBtn->getContentSize().width) / 2, exitBtn->getContentSize().height * 5));

    return true;
}

void Launcher::startGame()
{
	Director::getInstance()->pushScene(TransitionMoveInR::create(0.5f, SaveSelect::createScene(SaveSelect::SAVE_START)));
}

void Launcher::loadGame()
{
	Director::getInstance()->pushScene(TransitionMoveInR::create(0.5f, SaveSelect::createScene(SaveSelect::SAVE_LOAD)));
}

void Launcher::settingGame()
{
	Director::getInstance()->pushScene(TransitionMoveInR::create(0.5f, HelpUI::createScene()));
}

void Launcher::exitGame()
{
	Director::getInstance()->popScene();
}
