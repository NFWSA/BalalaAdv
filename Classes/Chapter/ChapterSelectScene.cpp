#include "ChapterSelectScene.h"
#include "ui\CocosGUI.h"
#include "..\SaveSelectScene.h"
#include "01-KanColle\Scene01.h"
#include "02-MyLittlePony\Scene02.h"
#include "03-LoveLive\Scene03.h"
#include "04-EastProject\Scene04.h"
#include "05-SOX\Scene05.h"
#include "DialogLayer.h"

#include "01-KanColle\FinalBoss01.h"
#include "02-MyLittlePony\FinalBoss02.h"
#include "03-LoveLive\FinalBoss03.h"
#include "04-EastProject\FinalBoss04.h"

USING_NS_CC;

Scene* ChapterSelect::createScene(int num, int saveType)
{
	auto scene = Scene::create();
	auto layer = ChapterSelect::create();
	layer->loadSave(num, saveType);
	scene->addChild(layer);
	return scene;
}

bool ChapterSelect::init()
{
	if (!Layer::init()){
		return false;
	}

	auto background = experimental::TMXTiledMap::create("Map/ChapterSelect.tmx");
	addChild(background);
	auto objects = background->getObjectGroup("Objects");

	for (int i = 0; i < 5; ++i){
		_chapter[i] = ui::Button::create("Map/Tiles/Chapter" + Value(i + 1).asString() + ".png", "Map/Tiles/Chapter" + Value(i + 1).asString() + "_Select.png");
		addChild(_chapter[i]);
		auto pos = objects->getObject("Chapter" + Value(i + 1).asString());
		_chapter[i]->setPosition(Vec2(pos["x"].asInt(), pos["y"].asInt()));
		_chapter[i]->addClickEventListener(CC_CALLBACK_1(ChapterSelect::chapterSelect, this, i + 1));
	}

	_return = ui::Button::create("UI/Return.png", "UI/Return_Click.png");
	addChild(_return);
	_return->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, _return->getContentSize().height / 2 + 10));
	_return->addClickEventListener(CC_CALLBACK_1(ChapterSelect::returnLauncher, this));

	return true;
}

void ChapterSelect::returnLauncher(Ref *ref)
{
	Director::getInstance()->popScene();
}

void ChapterSelect::loadSave(int num, int saveType)
{
	auto fileu = FileUtils::getInstance();
	if (-1 == num){
		saveType = SaveSelect::SAVE_LOAD;
		ValueMap nowSave = fileu->getValueMapFromFile("../nowSave.dat");
		num = nowSave["nowSave"].asInt();
	}
	else{
		ValueMap nowSave;
		nowSave["nowSave"] = num;
		fileu->writeToFile(nowSave, fileu->fullPathForFilename("../nowSave.dat"));
	}
	if (SaveSelect::SAVE_LOAD == saveType){
		_saveData = FileUtils::getInstance()->getValueMapFromFile(std::string("Save/") + static_cast<char>('0' + num) + ".sav");
		for (int i = _saveData["finished"].asInt(); i < 5; ++i)
			_chapter[i]->setVisible(false);
		return;
	}
	_saveData["finished"] = 1;
	for (int i = 1; i < 5; ++i)
		_chapter[i]->setVisible(false);

	auto path = fileu->fullPathForFilename("Save/default.sav");
	path.erase(path.size() - 11);
	fileu->writeToFile(_saveData, path + static_cast<char>('0' + num) + ".sav");

	auto dialog = DialogLayer::createWithFile("Story-00.dat");
	addChild(dialog);
}

void ChapterSelect::chapterSelect(cocos2d::Ref *ref, int num)
{
	Scene *scene;
	switch (num){
		case 1:
			scene = ChapterScene01::createScene();
			//scene = FinalBoss01::createScene();
			break;
		case 2:
			scene = ChapterScene02::createScene();
			//scene = FinalBoss02::createScene();
			break;
		case 3:
			scene = ChapterScene03::createScene();
			//scene = FinalBoss03::createScene();
			break;
		case 4:
			scene = ChapterScene04::createScene();
			//scene = FinalBoss04::createScene();
			break;
		case 5:
			scene = ChapterScene05::createScene();
			break;
		default:
			scene = ChapterScene01::createScene();
			break;
	}
	Director::getInstance()->replaceScene(TransitionFade::create(2.0f, scene));
}
