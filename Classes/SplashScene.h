#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"

class Splash : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

	void teamInfo();
	void gameInfo();

	void nextScene();
	bool touchNext(cocos2d::Touch *touch, cocos2d::Event *event);

    virtual bool init();

    CREATE_FUNC(Splash);
private:
	cocos2d::Label *_teamInfo, *_gameInfo;
	cocos2d::EventListenerTouchOneByOne *_pressNext;
};

#endif // __SPLASH_SCENE_H__
