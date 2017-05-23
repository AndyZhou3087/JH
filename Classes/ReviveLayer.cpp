#include "ReviveLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "DeathLayer.h"
#include "SoundManager.h"

ReviveLayer::ReviveLayer()
{

}


ReviveLayer::~ReviveLayer()
{
}

bool ReviveLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("reviveLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cacelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onCancel, this));

	cocos2d::ui::Button* revivebtn = (cocos2d::ui::Button*)csbnode->getChildByName("revivebtn");
	revivebtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onRevive, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

ReviveLayer* ReviveLayer::create()
{
	ReviveLayer *pRet = new ReviveLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

void ReviveLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		this->removeFromParentAndCleanup(true);
		if (g_gameLayer != NULL)
			g_gameLayer->removeChildByName("fightlayer");
		g_gameLayer->resume();
		DeathLayer* layer = DeathLayer::create();
		Director::getInstance()->getRunningScene()->addChild(layer);
	}
}
void ReviveLayer::onRevive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		this->removeFromParentAndCleanup(true);
		if (g_gameLayer != NULL)
			g_gameLayer->removeChildByName("fightlayer");

		g_gameLayer->heroRevive();
	}
}