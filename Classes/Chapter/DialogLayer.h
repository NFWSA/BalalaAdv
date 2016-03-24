#ifndef __DIALOG_LAYER_H__
#define __DIALOG_LAYER_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include <functional>

class DialogLayer : public cocos2d::Layer
{
public:
	static DialogLayer* createWithFile(const std::string &str);

    virtual bool init();
	virtual void onExitTransitionDidStart();
	virtual void onExit();

	bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);

	void setDataFile(const std::string &str);

	std::function<void()> onEnded;
private:
	cocos2d::EventListenerTouchOneByOne *_touchEvent;
	cocos2d::Sprite *_this, *_that, *_thisBox, *_thatBox;
	cocos2d::Label *_thisSpeech, *_thatSpeech;
	cocos2d::ValueMap _data;
	int _id, _num;
};

#endif // __DIALOG_LAYER_H__
