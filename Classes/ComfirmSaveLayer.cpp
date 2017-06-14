﻿#include "ComfirmSaveLayer.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "GameDataSave.h"
#include "Utility.h"
#include "SelectHeroScene.h"
bool ComfirmSaveLayer::init()
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	Node* csbnode = CSLoader::createNode("comfirmSaveLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(ComfirmSaveLayer::onOk, this));

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(ComfirmSaveLayer::onCancel, this));

	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	//点击任何位置移除掉
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		return;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}


void ComfirmSaveLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		saveUserIds();
		removSelf();
		Scene* scene = SelectHeroScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void ComfirmSaveLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

		removSelf();
	}
}

void ComfirmSaveLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

void ComfirmSaveLayer::saveUserIds()
{
	std::string uid = GameDataSave::getInstance()->getUserId();
	GlobalData::setUId(uid);
	int heroid = GameDataSave::getInstance()->getHeroId();
	std::vector<std::string> vec_ids = GlobalData::getSaveListId();

	vec_ids[heroid - 1] = uid;

	GlobalData::setSaveListId(vec_ids);
}

