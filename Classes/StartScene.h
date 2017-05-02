﻿#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class StartScene : public cocos2d::Layer
{
public:
	StartScene();
	~StartScene();
    static cocos2d::Scene* createScene();

	virtual void onExit();
    virtual bool init();
    
	CREATE_FUNC(StartScene);
private:
	void onNewStart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onLoadSaved(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:


};

#endif