#ifndef __LAUNCHER_SCENE_H__
#define __LAUNCHER_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Launcher : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

	void startGame();
	void loadGame();
	void settingGame();
	void exitGame();

    virtual bool init();
	virtual void onEnterTransitionDidFinish();

    CREATE_FUNC(Launcher);
private:
	cocos2d::ui::Button *_loadBtn;
};

#endif // __LAUNCHER_SCENE_H__
