#ifndef __UILINKER_H__
#define __UILINKER_H__

#include "cocos2d.h"
#include "GameUILayer.h"
#include "Character\Player.h"
#include "TypeDefine.h"

void linkUIPlayer(GameUI *layer, Player *player);
void linkMapPlayer(MapType *map, Player *player);
void setBackground(cocos2d::Sprite *bg, cocos2d::Node *scene);
void linkGameOver(cocos2d::Layer *scene);
void linkGamePass(cocos2d::Layer *scene, std::function<void(cocos2d::EventCustom*)> gamePass);
void gamePassSave(int no);
void linkGameBranch(cocos2d::Layer *scene, std::function<void(cocos2d::EventCustom*)> gameBranch);
void linkDialogEvent(cocos2d::Layer *scene, const std::string &str, std::function<void()> dialogEnded);

void pauseEnemys(Player *player);
void resumeEnemys(Player *player);
void removeHideData(const std::string &str);
void writeHideData(const std::string &str, bool flag = false);
bool isUsedHideData(const std::string &str);
bool existHideData(const std::string &str);
void damageAllEnemys(Player *player, int damage);

#endif // __UILINKER_H__
