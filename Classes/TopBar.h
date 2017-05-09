﻿#ifndef _TOPBAR_H_
#define _TOPBAR_H_
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "UIScroll.h"
USING_NS_CC;

class TopBar:public Node
{
public:
	TopBar();
	~TopBar();

	virtual bool init();

	CREATE_FUNC(TopBar);

private:
	cocos2d::ui::ImageView* heroimg;
	cocos2d::ui::ImageView* reason;
	cocos2d::ui::ImageView* weather;
	cocos2d::ui::TextBMFont* livedayslbl;
	cocos2d::ui::TextBMFont* timelbl;
	cocos2d::ui::TextBMFont* templbl;
	float pastmin;//虚拟分钟，
	ProgressTimer* outinjuryBar;
	ProgressTimer* innerinjuryBar;
	ProgressTimer* hungerBar;
	ProgressTimer* spiritBar;
	ProgressTimer* lifeBar;
private:
	void onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updataUI(float dt);
};
#endif

