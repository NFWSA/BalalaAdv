#ifndef __GAMEUI_LAYER_H__
#define __GAMEUI_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>

class GameUI : public cocos2d::Layer
{
public:
	enum FRONT{ UP = 1, DOWN = 2, LEFT = 4, RIGHT = 8 };

	virtual bool init();

	bool panMoveBegan(cocos2d::Touch*, cocos2d::Event*);
	void panMoved(cocos2d::Touch*, cocos2d::Event*);
	void panMoveEnded(cocos2d::Touch*, cocos2d::Event*);

	bool screenTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	void screenTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	void screenTouchEnded(cocos2d::Touch*, cocos2d::Event*);

	void keyPressedEvent(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
	void keyReleasedEvent(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);

	void setHP(float dt);
	void setSpecial(cocos2d::EventCustom *event, bool flag);

	CREATE_FUNC(GameUI);

	std::function<void()> onUpBegan, onDownBegan, onLeftBegan, onRightBegan, onJumpBegan;
	std::function<void()> onUpEnded, onDownEnded, onLeftEnded, onRightEnded, onJumpEnded, onSpecial;
	std::function<bool()> onMode1, onMode2, onMode3;
	std::function<void(cocos2d::Touch*, cocos2d::Event*)> onClickMoved, onClickEnded;
private:
	cocos2d::Sprite *_pan, *_movePan, *_hpBox;
	cocos2d::ui::Button *_mode1Check, *_mode2Check, *_mode3Check;
	cocos2d::ui::Button *_jumpBtn, *_pauseBtn, *_specialBtn;
	cocos2d::ProgressTimer *_hpBar;
	int _front;
	cocos2d::EventListenerKeyboard *_keyListener;
	cocos2d::EventListenerCustom *_branchListener;
	cocos2d::EventListenerTouchOneByOne *_panListener, *_screenListener;
};

#endif // __GAMEUI_LAYER_H__
