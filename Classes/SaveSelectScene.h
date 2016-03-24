#ifndef __SAVESELECT_SCENE_H__
#define __SAVESELECT_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SaveSelect : public cocos2d::Layer
{
public:
	enum{ SAVE_START, SAVE_LOAD };

	static cocos2d::Scene* createScene(int type);

	virtual bool init();
	virtual void onExitTransitionDidStart();
	void setSaveType(int type);
	void selectSave(int num);

	bool touchBegan(cocos2d::Touch*, cocos2d::Event*);
	void touchEnded(cocos2d::Touch*, cocos2d::Event*);

	CREATE_FUNC(SaveSelect);
private:
	void returnLauncher(cocos2d::Ref*);

	int _saveType;
	cocos2d::Sprite *_save1, *_save2, *_save3;
	cocos2d::ui::Button *_return;
	cocos2d::EventListenerTouchOneByOne *_saveEvent;
};

#endif //__SAVESELECT_SCENE_H__
