#include "SaveSelectScene.h"
#include "ui\CocosGUI.h"
#include "cocostudio\CocoStudio.h"
#include "Chapter\ChapterSelectScene.h"

USING_NS_CC;

Scene* SaveSelect::createScene(int type)
{
	auto scene = Scene::create();
	auto layer = SaveSelect::create();
	layer->setSaveType(type);
	scene->addChild(layer);
	return scene;
}

bool SaveSelect::init()
{
	if (!Layer::init()){
		return false;
	}
	auto layer = Sprite::create("SaveSelect.png");
	this->addChild(layer);
	layer->setPosition(Director::getInstance()->getVisibleSize()/2);
	auto blur = LayerColor::create(Color4B(255, 174, 201, 200));
	this->addChild(blur);

	auto fileUtils = FileUtils::getInstance();

	_save1 = Sprite::create("SaveLayer.png");
	_save1->setAnchorPoint(Vec2(0, 0));
	_save1->setOpacity(220);
	_save1->setScaleX(Director::getInstance()->getVisibleSize().width / 4 / _save1->getContentSize().width);
	_save1->setScaleY(Director::getInstance()->getVisibleSize().height / 2 / _save1->getContentSize().height);
	_save1->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 12, Director::getInstance()->getVisibleSize().height / 4));
	auto label1 = Label::createWithSystemFont("No thing.", "Arial", 36);
	label1->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 5 / 24, Director::getInstance()->getVisibleSize().height / 2));
	if (fileUtils->isFileExist("Save/0.sav")){
		auto str = fileUtils->getValueMapFromFile("Save/0.sav");
		label1->setString(str["finished"].asString() + " / 5");
	}
	
	_save2 = Sprite::create("SaveLayer.png");
	_save2->setAnchorPoint(Vec2(0, 0));
	_save2->setOpacity(220);
	_save2->setScaleX(Director::getInstance()->getVisibleSize().width / 4 / _save2->getContentSize().width);
	_save2->setScaleY(Director::getInstance()->getVisibleSize().height / 2 / _save2->getContentSize().height);
	_save2->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 3 / 8, Director::getInstance()->getVisibleSize().height / 4));
	auto label2 = Label::createWithSystemFont("No thing.", "Arial", 36);
	label2->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
	if (fileUtils->isFileExist("Save/1.sav")){
		auto str = fileUtils->getValueMapFromFile("Save/1.sav");
		label2->setString(str["finished"].asString() + " / 5");
	}

	_save3 = Sprite::create("SaveLayer.png");
	_save3->setAnchorPoint(Vec2(0, 0));
	_save3->setOpacity(220);
	_save3->setScaleX(Director::getInstance()->getVisibleSize().width / 4 / _save3->getContentSize().width);
	_save3->setScaleY(Director::getInstance()->getVisibleSize().height / 2 / _save3->getContentSize().height);
	_save3->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height / 4));
	auto label3 = Label::createWithSystemFont("No thing.", "Arial", 36);
	label3->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 19 / 24, Director::getInstance()->getVisibleSize().height / 2));
	if (fileUtils->isFileExist("Save/2.sav")){
		auto str = fileUtils->getValueMapFromFile("Save/2.sav");
		label3->setString(str["finished"].asString() + " / 5");
	}

	_return = ui::Button::create("UI/Return.png", "UI/Return_Click.png");
	_return->addClickEventListener(CC_CALLBACK_1(SaveSelect::returnLauncher, this));
	_return->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - _return->getContentSize().width / 2 - 10, _return->getContentSize().height / 2 + 10));

	_saveEvent  = EventListenerTouchOneByOne::create();
	_saveEvent->onTouchBegan = CC_CALLBACK_2(SaveSelect::touchBegan, this);
	_saveEvent->onTouchEnded = CC_CALLBACK_2(SaveSelect::touchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_saveEvent, this);

	this->addChild(_save1);
	this->addChild(_save2);
	this->addChild(_save3);
	this->addChild(_return);

	this->addChild(label1);
	this->addChild(label2);
	this->addChild(label3);

	return true;
}

void SaveSelect::onExitTransitionDidStart()
{
	_eventDispatcher->removeEventListener(_saveEvent);
}

void SaveSelect::returnLauncher(cocos2d::Ref*)
{
	Director::getInstance()->popScene();
}

void SaveSelect::setSaveType(int type)
{
	_saveType = type;
}

void SaveSelect::selectSave(int num)
{
	auto scene = ChapterSelect::createScene(num, _saveType);
	auto transScene = TransitionPageTurn::create(1.0f, scene, false);
	Director::getInstance()->replaceScene(transScene);
}

bool SaveSelect::touchBegan(cocos2d::Touch*, cocos2d::Event*)
{
	return true;
}

void SaveSelect::touchEnded(cocos2d::Touch* touch, cocos2d::Event*)
{
	auto pos = touch->getLocation();
	auto fileUtils = FileUtils::getInstance();
	if (_save1->getBoundingBox().containsPoint(pos))
		if (SAVE_START == _saveType || (SAVE_LOAD == _saveType && fileUtils->isFileExist("Save/0.sav"))){
			selectSave(0);
			return;
		}
	if (_save2->getBoundingBox().containsPoint(pos))
		if (SAVE_START == _saveType || (SAVE_LOAD == _saveType && fileUtils->isFileExist("Save/1.sav"))){
			selectSave(1);
			return;
		}
	if (_save3->getBoundingBox().containsPoint(pos))
		if (SAVE_START == _saveType || (SAVE_LOAD == _saveType && fileUtils->isFileExist("Save/2.sav"))){
			selectSave(2);
			return;
		}
}
