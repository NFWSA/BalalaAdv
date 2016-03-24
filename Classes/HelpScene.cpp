#include "HelpScene.h"
#include "ui\CocosGUI.h"
#include "cocostudio\CocoStudio.h"

USING_NS_CC;

Scene* HelpUI::createScene()
{
	auto scene = Scene::create();
	auto layer = HelpUI::create();
	scene->addChild(layer);
	return scene;
}

bool HelpUI::init()
{
	if (!Layer::init()){
		return false;
	}
	auto layer = Sprite::create("HelpUI.png");
	this->addChild(layer);
	layer->setPosition(Director::getInstance()->getVisibleSize() / 2);
	auto blur = LayerColor::create(Color4B(255, 174, 201, 150));
	addChild(blur);

	auto scroll = ui::ScrollView::create();
	scroll->setInnerContainerSize(Size(Director::getInstance()->getVisibleSize().width - 40, Director::getInstance()->getVisibleSize().height * 2));
	scroll->setContentSize(Size(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height - 110));
	scroll->setPositionY(100);
	auto text = Label::createWithSystemFont("", "Î¢ÈíÑÅºÚ", 27);
	text->setDimensions(Director::getInstance()->getVisibleSize().width - 40, Director::getInstance()->getVisibleSize().height * 2);
	text->setAnchorPoint(Vec2(0.5, 1));
	text->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 2 - 40));
	text->setTextColor(Color4B::BLACK);
	text->setString(FileUtils::getInstance()->getStringFromFile("HelpInfo.dat"));
	scroll->addChild(text);
	addChild(scroll);
	
	_return = ui::Button::create("UI/Return.png", "UI/Return_Click.png");
	_return->addClickEventListener(CC_CALLBACK_1(HelpUI::returnLast, this));
	addChild(_return);
	_return->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 3, 10 + _return->getContentSize().height / 2));
	_home = ui::Button::create("UI/Home.png", "UI/Home_Click.png");
	_home->addClickEventListener(CC_CALLBACK_1(HelpUI::returnHome, this));
	addChild(_home);
	_home->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 2 / 3, 10 + _home->getContentSize().height / 2));

	return true;
}

void HelpUI::returnLast(cocos2d::Ref*)
{
	Director::getInstance()->popScene();
}

void HelpUI::returnHome(cocos2d::Ref*)
{
	Director::getInstance()->popToRootScene();
}