#include "BedUILayer.h"
#include "json.h"
#include "Bed.h"

BedUILayer::BedUILayer()
{

}


BedUILayer::~BedUILayer()
{
}

bool BedUILayer::init()
{
	Node* csbnode = CSLoader::createNode("bedUiLayer.csb");
	this->addChild(csbnode);
	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(BedUILayer::onBack, this));
	return true;
}

void BedUILayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}