#ifndef __CHAPTER_SCENE_01_H__
#define __CHAPTER_SCENE_01_H__

#include "cocos2d.h"
#include "../GameUILayer.h"
#include "../Character/Player.h"

class ChapterScene01 : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	void gamePass(cocos2d::EventCustom*);
	void gameBranch(cocos2d::EventCustom *event);

	virtual bool init();
	virtual void onExitTransitionDidStart();

	void onDialogEnded();

	CREATE_FUNC(ChapterScene01);
private:
	Player *_player;
};

#endif //__CHAPTER_SCENE_01_H__
