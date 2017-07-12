﻿#include "StartScene.h"
#include "SelectHeroScene.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "SettingLayer.h"
#include "SoundManager.h"
#include "SelectSaveLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iosfunc.h"
#endif

USING_NS_CC;

StartScene::StartScene()
{

}
StartScene::~StartScene()
{

}

Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	StartScene* startlayer = StartScene::create();

    // add layer as a child to scene
	scene->addChild(startlayer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Node* csbnode = CSLoader::createNode("startLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* newstartbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("newstartbtn");
	newstartbtn->addTouchEventListener(CC_CALLBACK_2(StartScene::onNewStart, this));


	cocos2d::ui::Widget* savedbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("savedbtn");
	savedbtn->addTouchEventListener(CC_CALLBACK_2(StartScene::onLoadSaved, this));

	cocos2d::ui::Widget* qq = (cocos2d::ui::Widget*)csbnode->getChildByName("qq");
	qq->addTouchEventListener(CC_CALLBACK_2(StartScene::onQQ, this));

	cocos2d::ui::Widget* continuebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("continuebtn");
	continuebtn->addTouchEventListener(CC_CALLBACK_2(StartScene::onContinue, this));
	std::string uid = GlobalData::getUId();

	continuebtn->setEnabled(uid.length() <= 0?false:true);

	cocos2d::ui::Widget* setbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("setbtn");
	setbtn->addTouchEventListener(CC_CALLBACK_2(StartScene::onSet, this));

	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_START);
    return true;
}

void StartScene::onExit()
{
	Layer::onExit();
}

void StartScene::onNewStart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{	
		Scene* scene = SelectHeroScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void StartScene::onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string uid = GlobalData::getUId();
		GlobalData::setUId(uid);

		Scene* scene = GameScene::createScene();

		Director::getInstance()->replaceScene(scene);
	}
}

void StartScene::onLoadSaved(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->addChild(SelectSaveLayer::create());
	}
}

void StartScene::onSet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SettingLayer* layer = SettingLayer::create();
		addChild(layer);
	}
}

void StartScene::onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		cocos2d::ui::Text* qq = (cocos2d::ui::Text*)pSender; 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		copytoclipboard((char*)qq->getString().c_str());
#endif
	}

}
