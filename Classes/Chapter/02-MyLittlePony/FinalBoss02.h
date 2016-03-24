#ifndef __FINALBOSS_SCENE_02_H__
#define __FINALBOSS_SCENE_02_H__

#include "cocos2d.h"
#include "../Character/Player.h"

class FinalBoss02 : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	void gamePass(cocos2d::EventCustom*);
	void onSpecial();
	void onDialogEnded();

	virtual bool init();
	virtual void onExitTransitionDidStart();

	CREATE_FUNC(FinalBoss02);
private:
	Player *_player;
};

#endif //__FINALBOSS_SCENE_02_H__
