#include "CreditScene.h"
#include "ui\CocosGUI.h"
#include "cocostudio\CocoStudio.h"

USING_NS_CC;

Scene* CreditScene::createScene(const std::string &infoFile, const std::string &texFile)
{
	auto scene = Scene::create();
	auto layer = CreditScene::createWithFile(infoFile, texFile);
	scene->addChild(layer);
	return scene;
}

CreditScene* CreditScene::createWithFile(const std::string &infoFile, const std::string &texFile)
{
	CreditScene *pRet = new(std::nothrow) CreditScene();
	if (pRet&&pRet->init()){
		pRet->autorelease();
		auto file = FileUtils::getInstance()->getValueMapFromFile(infoFile);
		for (int i = 0; i < file["number"].asInt(); ++i){
			pRet->addInfo(file["info" + Value(i).asString()].asString());
		}
		pRet->addPic(texFile);
		pRet->show();
		return pRet;
	}
	else{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool CreditScene::init()
{
	if (!Layer::init()){
		return false;
	}

	_background = Sprite::create();
	addChild(_background);
	_background->setPosition(Director::getInstance()->getVisibleSize() / 2);

	auto layer = LayerColor::create(Color4B(0, 0, 0, 180));
	addChild(layer);

	_label = Label::createWithSystemFont("", "Arial", 36);
	addChild(_label);
	_label->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 0));

	_id = 0;

	return true;
}

void CreditScene::addInfo(const std::string &str)
{
	_label->setString(_label->getString() + '\n' + str);
}

void CreditScene::addPic(const std::string &filename)
{
	_file = FileUtils::getInstance()->getValueMapFromFile(filename);
	changeBG();
}

void CreditScene::show()
{
	_label->setPositionY(-_label->getContentSize().height / 2);
	auto action = MoveTo::create(_label->getContentSize().height / Director::getInstance()->getVisibleSize().height * 6.0f,
													Vec2(_label->getPositionX(), _label->getContentSize().height / 2 + Director::getInstance()->getVisibleSize().height / 2));
	auto delay = DelayTime::create(3.0f);
	auto seq = Sequence::create(action, delay, CallFunc::create([](){
		Director::getInstance()->popScene();
	}), nullptr);
	_label->runAction(seq);
}

void CreditScene::changeBG()
{
	if (_file["number"].asInt() == _id){
		_id = 0;
	}
	_background->setTexture(_file["texture" + Value(_id).asString()].asString());
	_background->setScaleX(Director::getInstance()->getVisibleSize().width / _background->getContentSize().width);
	_background->setScaleY(Director::getInstance()->getVisibleSize().height / _background->getContentSize().height);
	auto in = FadeIn::create(1.0f);
	auto delay = DelayTime::create(4.0f);
	auto out = FadeOut::create(1.0f);
	auto seq = Sequence::create(in, delay, out, CallFunc::create(this, SEL_CallFunc(&CreditScene::changeBG)), nullptr);
	_background->runAction(seq);
	++_id;
}
