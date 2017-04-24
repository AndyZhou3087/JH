#include "HintBox.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
bool HintBox::init(std::string text)
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	Node* csbnode = CSLoader::createNode("hintLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(HintBox::onOk, this));

	cocos2d::ui::Text* content = (cocos2d::ui::Text*)csbnode->getChildByName("content");
	content->setString(text);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		//removSelf();
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

HintBox* HintBox::create(std::string content)
{
	HintBox *pRet = new HintBox();
	if (pRet && pRet->init(content))
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

void HintBox::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void HintBox::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

