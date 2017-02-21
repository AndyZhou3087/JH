#include "TopBar.h"

TopBar::TopBar()
{
}


TopBar::~TopBar()
{
}

bool TopBar::init()
{
	Node* csbnode = CSLoader::createNode("topBarNode.csb");
	this->addChild(csbnode);

	cocos2d::ui::ImageView* heroimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroimg");
	heroimg->setName("hero");
	heroimg->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* reason = (cocos2d::ui::ImageView*)csbnode->getChildByName("reason");
	reason->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));
	reason->setName("reason");

	cocos2d::ui::ImageView* wheather = (cocos2d::ui::ImageView*)csbnode->getChildByName("wheather");
	wheather->setName("wheather");
	wheather->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* livedaysincon = (cocos2d::ui::ImageView*)csbnode->getChildByName("livedaysincon");
	livedaysincon->setName("livedays");
	livedaysincon->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* timeicon = (cocos2d::ui::ImageView*)csbnode->getChildByName("toptimeicon");
	timeicon->setName("time");
	timeicon->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* temperature = (cocos2d::ui::ImageView*)csbnode->getChildByName("toptemperature");
	temperature->setName("temperature");
	temperature->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));


	cocos2d::ui::TextBMFont* livedayslbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("livedayslbl");
	livedayslbl->setName("livedays");
	livedayslbl->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::TextBMFont* timelbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("timelbl");
	timelbl->setName("time");
	timelbl->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::TextBMFont* templbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("templbl");
	templbl->setName("temperature");
	templbl->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* outinjury = (cocos2d::ui::ImageView*)csbnode->getChildByName("topoutinjurybg");
	outinjury->setName("outinjury");
	outinjury->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* innerinjury = (cocos2d::ui::ImageView*)csbnode->getChildByName("topinnerinjurybg");
	innerinjury->setName("innerinjury");
	innerinjury->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* hunger = (cocos2d::ui::ImageView*)csbnode->getChildByName("tophungerbg");
	hunger->setName("hunger");
	hunger->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* spirit = (cocos2d::ui::ImageView*)csbnode->getChildByName("topspiritbg");
	spirit->setName("spirit");
	spirit->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* life = (cocos2d::ui::ImageView*)csbnode->getChildByName("toplifebg");
	life->setName("life");
	life->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	return true;
}

void TopBar::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* cnode = (Node*)pSender;
		if (cnode->getName().compare("hero") == 0)
		{

		}
		else if (cnode->getName().compare("reason") == 0)
		{

		}
		else if (cnode->getName().compare("wheather") == 0)
		{

		}
		else if (cnode->getName().compare("livedays") == 0)
		{

		}
		else if (cnode->getName().compare("time") == 0)
		{

		}
		else if (cnode->getName().compare("temperature") == 0)
		{

		}
		else if (cnode->getName().compare("outinjury") == 0)
		{

		}
		else if (cnode->getName().compare("innerinjury") == 0)
		{

		}
		else if (cnode->getName().compare("hunger"))
		{

		}
		else if (cnode->getName().compare("spirit"))
		{

		}
		else if (cnode->getName().compare("life"))
		{

		}

	}
}
