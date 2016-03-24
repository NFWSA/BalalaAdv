#ifndef __FINALBOSS_SCENE_01_H__
#define __FINALBOSS_SCENE_01_H__

#include "cocos2d.h"
#include "../Character/Player.h"

class FinalBoss01 : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	void gamePass(cocos2d::EventCustom*);

	virtual bool init();
	virtual void onExitTransitionDidStart();

	void onDialogEnded();
	void onSpecial();

	CREATE_FUNC(FinalBoss01);
private:
	Player *_player;
};

#endif //__FINALBOSS_SCENE_01_H__
