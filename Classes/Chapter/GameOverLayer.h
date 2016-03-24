#ifndef __GAMEOVER_LAYER_H__
#define __GAMEOVER_LAYER_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Layer
{
public:
	virtual bool init();
	virtual void onEnterTransitionDidFinish();

	void onClick(cocos2d::Ref*);

	CREATE_FUNC(GameOver);

	std::function<cocos2d::Scene*()> onRestart;
private:
	cocos2d::ui::Button *_next;
};

#endif // __GAMEOVER_LAYER_H__
