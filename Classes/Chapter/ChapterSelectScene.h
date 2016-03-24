#ifndef __CHAPTERSELECT_SCENE_H__
#define __CHAPTERSELECT_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"

class ChapterSelect : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene(int num = -1, int saveType = -1);

	virtual bool init();

	void loadSave(int num, int saveType);
	void chapterSelect(cocos2d::Ref *ref, int num);

	CREATE_FUNC(ChapterSelect);
private:
	void returnLauncher(cocos2d::Ref*);

	cocos2d::ValueMap _saveData;
	cocos2d::ui::Button *_chapter[5], *_return;
};

#endif //__CHAPTERSELECT_SCENE_H__
