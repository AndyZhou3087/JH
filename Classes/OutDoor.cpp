#include "OutDoor.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "MapLayer.h"
#include "HeroProperNode.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "GlobalData.h"

OutDoor::OutDoor()
{

}


OutDoor::~OutDoor()
{
}

bool OutDoor::init()
{
	m_csbnode = CSLoader::createNode("outDoorLayer.csb");
	this->addChild(m_csbnode);

	Node* heroproper = HeroProperNode::create();
	heroproper->setPosition(Vec2(360, 790));
	m_csbnode->addChild(heroproper);

	scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");

	updata();

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(OutDoor::onBack, this));


	cocos2d::ui::Button* outbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("outbtn");
	outbtn->addTouchEventListener(CC_CALLBACK_2(OutDoor::onOut, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

void OutDoor::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		for (int i = 0; i < MyPackage::getSize(); i++)
		{
			StorageRoom::add(MyPackage::vec_packages[i]);
		}
		MyPackage::takeoff();
		this->removeFromParentAndCleanup(true);
	}
}

void OutDoor::onOut(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		g_maplayer = MapLayer::create();
		g_gameLayer->addChild(g_maplayer, 1, "maplayer");
		g_gameLayer->removeChildByName("homelayer");

		this->removeFromParentAndCleanup(true);

	}
}

void OutDoor::updata()
{
	int typecount = 0;
	for (int i = 0; i < RES_MAX; i++)
	{
		typecount += StorageRoom::map_storageData[i].size();
	}

	int row = typecount % 5 == 0 ? typecount / 5 : (typecount / 5 + 1);

	int innerheight = row * 130;
	int contentheight = scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollview->setInnerContainerSize(Size(650, innerheight));

	allStorageData.clear();
	for (int i = 0; i < RES_MAX; i++)
	{
		for (unsigned int m = 0; m < StorageRoom::map_storageData[i].size(); m++)
		{
			allStorageData.push_back(&StorageRoom::map_storageData[i][m]);
		}
	}
	for (unsigned int i = 0; i < allStorageData.size();i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");
		//box->setPosition(Vec2(box->getContentSize().width/2 + 20 + m % 5 * 120, sepline->getPositionY() - 5 - 65 - m/5*130));
		//scrollview->addChild(box);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(OutDoor::onStorageItem, this));
		boxItem->setUserData(allStorageData[i]);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width/2 + 10 + i % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - i / 5 * 130));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		scrollview->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", allStorageData[i]->strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", allStorageData[i]->count);
		Label * reslbl = Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}

	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(OutDoor::onPackageItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(110 + i * 125, 530));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", MyPackage::vec_packages[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", MyPackage::vec_packages[i].count);
		Label * reslbl = Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void OutDoor::onStorageItem(cocos2d::Ref* pSender)
{
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();

	removeitem();

	int count = data->count - 1;
	if (count <= 0)
	{
		std::vector<PackageData>::iterator it;
		for (it = StorageRoom::map_storageData[data->type].begin(); it != StorageRoom::map_storageData[data->type].end(); ++it)
		{
			if (it->strid.compare(data->strid) == 0)
			{
				PackageData pdata;
				pdata.type = data->type;
				pdata.strid = data->strid;
				pdata.count = 1;
				pdata.lv = data->lv;
				pdata.extype = data->extype;
				if (MyPackage::add(pdata) == 0)
				{
					data->count--;
					StorageRoom::map_storageData[data->type].erase(it);
				}
				break;
			}
		}
	}
	else
	{
		PackageData pdata;
		pdata.type = data->type;
		pdata.strid = data->strid;
		pdata.count = 1;
		pdata.lv = data->lv;
		pdata.extype = data->extype;
		if (MyPackage::add(pdata) == 0)
		{
			data->count--;
		}
	}

	updata();
}

void OutDoor::onPackageItem(cocos2d::Ref* pSender)
{
	removeitem();
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = MyPackage::vec_packages[index];
	data.count = 1;
	StorageRoom::add(data);
	MyPackage::cutone(index);
	updata();
	
}

void OutDoor::removeitem()
{
	for (unsigned int i = 0; i < allStorageData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		scrollview->removeChildByName(name);
	}

	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		this->removeChildByName(name);
	}
}