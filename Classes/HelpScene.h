#ifndef __HELP_SCENE_H__
#define __HELP_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class HelpUI : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(HelpUI);
private:
	void returnLast(cocos2d::Ref*);
	void returnHome(cocos2d::Ref*);

	cocos2d::ui::Button *_return, *_home;
};

#endif //__HELP_SCENE_H__
