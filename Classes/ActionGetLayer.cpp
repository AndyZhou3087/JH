#include "ActionGetLayer.h"
#include "json.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "MyPackage.h"

std::string acname[] = {"战斗","采集", "砍伐", "挖掘" };
ActionGetLayer::ActionGetLayer()
{
}


ActionGetLayer::~ActionGetLayer()
{
}

bool ActionGetLayer::init(std::vector<int> vec_id, int type, int actype)
{
	Node* csbnode = CSLoader::createNode("actionGetLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onBack, this));

	cocos2d::ui::Widget* getbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onGet, this));

	cocos2d::ui::Widget* getallbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("allgetbtn");
	getallbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onAllGet, this));

	rewardRes_ids = vec_id;

	for (unsigned int i = 0; i < vec_id.size(); i++)
	{
		unsigned int m = 0;
		for ( m = 0; m < GlobalData::vec_getResData.size(); m++)
		{
			if (vec_id[i] == GlobalData::vec_getResData[m].id)
			{
				GlobalData::vec_getResData[m].count++;
				break;
			}
		}

		if (m == GlobalData::vec_getResData.size())
		{
			PackageData data;
			data.id = vec_id[i];
			data.type = type;
			data.count = 1;
			GlobalData::vec_getResData.push_back(data);
		}
		
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(ActionGetLayer::onRewardItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(150 + i * 135, 512));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		this->addChild(menu, 0, name );

		std::string str = StringUtils::format("ui/%d.png", vec_id[i]);
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		int count = 0;
		for (unsigned m = 0; m < GlobalData::vec_getResData.size(); m++)
		{
			if (vec_id[i] == GlobalData::vec_getResData[m].id)
				count = GlobalData::vec_getResData[m].count;
		}
		str = StringUtils::format("%d", count);
		Label * reslbl = Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void ActionGetLayer::onRewardItem(cocos2d::Ref* pSender)
{

}

ActionGetLayer* ActionGetLayer::create(std::vector<int> vec_id, int type, int actype)
{
	ActionGetLayer *pRet = new ActionGetLayer();
	if (pRet && pRet->init(vec_id, type, actype))
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

void ActionGetLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void ActionGetLayer::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

	}
}

void ActionGetLayer::onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

	}
}

void ActionGetLayer::updata()
{

}