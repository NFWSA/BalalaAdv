#ifndef __CREDIT_SCENE_H__
#define __CREDIT_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class CreditScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(const std::string &infoFile, const std::string &texFile);
	static CreditScene* createWithFile(const std::string &infoFile, const std::string &texFile);

	virtual bool init();

	void addInfo(const std::string &str);
	void addPic(const std::string &filename);
	void show();
private:
	void changeBG();

	cocos2d::Label *_label;
	cocos2d::Sprite *_background;
	cocos2d::ValueMap _file;
	int _id;
};

#endif //__CREDIT_SCENE_H__
