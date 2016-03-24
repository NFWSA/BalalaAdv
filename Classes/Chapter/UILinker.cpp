#include "cocostudio/CocoStudio.h"
#include "UILinker.h"
#include "Character/Enemy.h"
#include "Character/FlyEnemy.h"
#include "Character/BossEnemy.h"
#include "Character/BranchEnemy.h"
#include "Character/ShootEnemy.h"
#include "Character/FlyShootEnemy.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GameOverLayer.h"
#include "DialogLayer.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocostudio::timeline;

void linkUIPlayer(GameUI *layer, Player *player)
{
	layer->onRightBegan = CC_CALLBACK_0(Player::onRightBegan, player);
	layer->onRightEnded = CC_CALLBACK_0(Player::onRightEnded, player);
	layer->onLeftBegan = CC_CALLBACK_0(Player::onLeftBegan, player);
	layer->onLeftEnded = CC_CALLBACK_0(Player::onLeftEnded, player);
	layer->onJumpBegan = CC_CALLBACK_0(Player::onJumpBegan, player);
	layer->onJumpEnded = CC_CALLBACK_0(Player::onJumpEnded, player);
	layer->onMode1 = CC_CALLBACK_0(Player::onMode1, player);
	layer->onMode2 = CC_CALLBACK_0(Player::onMode2, player);
	layer->onMode3 = CC_CALLBACK_0(Player::onMode3, player);
	layer->onClickMoved = CC_CALLBACK_2(Player::onClickMoved, player);
	layer->onClickEnded = CC_CALLBACK_2(Player::onClickEnded, player);

	player->setHP = CC_CALLBACK_1(GameUI::setHP, layer);
}

void pauseEnemys(Player *player)
{
	for (auto i : player->_enemys){
		i->setPause(true);
	}
}

void resumeEnemys(Player *player)
{
	for (auto i : player->_enemys){
		i->setPause(false);
	}
}

void linkMapPlayer(MapType *map, Player *player)
{
	player->setTMXMap(map);
	map->getLayer("Meta")->setVisible(false);
	
	auto objects = map->getObjectGroup("Objects")->getObjects();
	for (auto i : objects){
		auto j = i.asValueMap();
		if ("Enemy" == j["name"].asString()){
			Enemy *k = nullptr;
			std::string str(j["type"].asString());
			if (str == "Normal")
					k = Enemy::create();
			if (str == "Shoot")
					k = ShootEnemy::create();
			if (str == "Fly")
				k = FlyEnemy::create();
			if (str == "FlyShoot")
				k = FlyShootEnemy::create();
			if (str == "Branch")
				k = BranchEnemy::create();
			if (str == "Boss")
				k = BossEnemy::create();
			k->setTexture(j["Texture"].asString());
			k->setTMXMap(map, j);
			k->setPlayer(player);
			std::string attEf("Sound/Effect/" + j["AttEffect"].asString()), deadEf("Sound/Effect/" + j["DeadEffect"].asString());
			k->setEffect(attEf, deadEf);
			if ("" != j["BallTexture"].asString()){
				std::string ballTex("Character/" + j["BallTexture"].asString());
				k->setBallTexture(ballTex);
			}
			if (str == "Boss"){
				std::string aiFile("AI/" + j["AI"].asString());
				k->setBallTexture(aiFile);
			}

			SimpleAudioEngine::getInstance()->preloadEffect(attEf.c_str());
			SimpleAudioEngine::getInstance()->preloadEffect(deadEf.c_str());
			player->getParent()->addChild(k);
			player->_enemys.pushBack(k);
			k->schedule(k->_onAISel, 0.025f);
			k->setPause(true);
		}
	}
}

void setBackground(cocos2d::Sprite *bg, cocos2d::Node *scene)
{
	bg->setPosition(Director::getInstance()->getVisibleSize() / 2);
	bg->setScaleX(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);
	bg->setScaleY(Director::getInstance()->getVisibleSize().height / bg->getContentSize().height);
	scene->addChild(bg);
}

void gameOver(EventCustom *event)
{
	auto scene = Scene::create();
	auto layer = GameOver::create();
	scene->addChild(layer);

	Director::getInstance()->replaceScene(scene);
}

void linkGameOver(cocos2d::Layer *scene)
{
	auto overEvent = EventListenerCustom::create("GameOver", gameOver);
	scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(overEvent, scene);
}

void linkGamePass(cocos2d::Layer *scene, std::function<void(EventCustom* event)> gamePass)
{
	auto passEvent = EventListenerCustom::create("GamePass", gamePass);
	scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(passEvent, scene);
}

void gamePassSave(int no)
{
	auto fileu = FileUtils::getInstance();
	ValueMap nowSave = fileu->getValueMapFromFile("../nowSave.dat");
	auto num = nowSave["nowSave"].asInt();

	auto saveData = fileu->getValueMapFromFile(std::string("Save/") + static_cast<char>('0' + num) + ".sav");
	if (no == saveData["finished"].asInt())
		saveData["finished"] = saveData["finished"].asInt() + 1;
	fileu->writeToFile(saveData, fileu->fullPathForFilename(std::string("Save/") + static_cast<char>('0' + num) + ".sav"));
}

void linkGameBranch(cocos2d::Layer *scene, std::function<void(cocos2d::EventCustom*)> gameBranch)
{
	auto branchEvent = EventListenerCustom::create("GameBranch", gameBranch);
	scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(branchEvent, scene);
}

void linkDialogEvent(cocos2d::Layer *scene, const std::string &str, std::function<void()> dialogEnded)
{
	auto dialog = DialogLayer::createWithFile(str);
	scene->addChild(dialog, 1);
	dialog->onEnded = dialogEnded;
}

void removeHideData(const std::string &str)
{
	auto fileU = FileUtils::getInstance();
	if (fileU->isFileExist("ChapterScene-" + str + ".dat"))
		fileU->removeFile(fileU->fullPathForFilename("ChapterScene-" + str + ".dat"));
}

void writeHideData(const std::string &str, bool flag)
{
	ValueMap data;
	data["HideStory"] = flag;
	auto fileU = FileUtils::getInstance();
	auto path = fileU->fullPathForFilename("ChapterScene-default.dat");
	path.erase(path.size() - 11);
	fileU->writeToFile(data, path + str + ".dat");
}

bool isUsedHideData(const std::string &str)
{
	if (existHideData(str)){
		auto fileU = FileUtils::getInstance();
		auto data = fileU->getValueMapFromFile("ChapterScene-" + str + ".dat");
		return data["HideStory"].asBool();
	}
	return true;
}

bool existHideData(const std::string &str)
{
	auto fileU = FileUtils::getInstance();
	auto path = fileU->fullPathForFilename("ChapterScene-default.dat");
	path.erase(path.size() - 11);
	return fileU->isFileExist(path + str + ".dat");
}

void damageAllEnemys(Player *player, int damage)
{
	Vector<Enemy*> deadEnemy;
	for (auto i : player->_enemys){
		if (i->getDamage(damage)){
			deadEnemy.pushBack(i);
		}
	}
	for (auto i : deadEnemy){
		(player->_enemys).eraseObject(i, true);
	}
}
