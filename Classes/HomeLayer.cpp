#include "HomeLayer.h"
#include "json.h"
#include "Bed.h"
#include "Furnace.h"
#include "Forgingtable.h"
#include "CookTable.h"
#include "MedicineKit.h"
#include "WineTable.h"
#include "WineMaker.h"
#include "StorageRoom.h"
#include "Fence.h"
#include "BuildingUILayer.h"
#include "StorageUILayer.h"
#include "OutDoor.h"
#include "GameScene.h"

HomeLayer::HomeLayer()
{
}


HomeLayer::~HomeLayer()
{
}

bool HomeLayer::init()
{
	Node* csbnode = CSLoader::createNode("homeLayer.csb");
	this->addChild(csbnode);

	Building* bed = Bed::create();
	Vec_Buildings.push_back(bed);

	MenuItemSprite* bedItem = MenuItemSprite::create(
		bed,
		bed,
		bed,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	bedItem->setName("bed");
	bedItem->setTag(0);
	bedItem->setPosition(Vec2(160, 700));
	Menu* menu = Menu::create();
	menu->addChild(bedItem);
	menu->setPosition(Vec2(0, 0));
	csbnode->addChild(menu);

	Building* cooktable = CookTable::create();
	Vec_Buildings.push_back(cooktable);
	MenuItemSprite* cooktableItem = MenuItemSprite::create(
		cooktable,
		cooktable,
		cooktable,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	cooktableItem->setName("cooktable");
	cooktableItem->setTag(1);
	cooktableItem->setPosition(Vec2(580, 786));
	menu->addChild(cooktableItem);

	Building* forgingtable = Forgingtable::create();
	Vec_Buildings.push_back(forgingtable);
	MenuItemSprite* forgingtableItem = MenuItemSprite::create(
		forgingtable,
		forgingtable,
		forgingtable,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	forgingtableItem->setName("forgingtable");
	forgingtableItem->setTag(2);
	forgingtableItem->setPosition(Vec2(195, 410));
	menu->addChild(forgingtableItem);

	Building* winemaker = WineMaker::create();
	Vec_Buildings.push_back(winemaker);
	MenuItemSprite* winemakerItem = MenuItemSprite::create(
		winemaker,
		winemaker,
		winemaker,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	winemakerItem->setName("winemaker");
	winemakerItem->setTag(3);
	winemakerItem->setPosition(Vec2(445, 800));
	menu->addChild(winemakerItem);

	Building* medicinekit = MedicineKit::create();
	Vec_Buildings.push_back(medicinekit);
	MenuItemSprite* medicinekitItem = MenuItemSprite::create(
		medicinekit,
		medicinekit,
		medicinekit,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	medicinekitItem->setName("medicinekit");
	medicinekitItem->setTag(4);
	medicinekitItem->setPosition(Vec2(678, 625));
	menu->addChild(medicinekitItem);


	Building* furnace = Furnace::create();
	Vec_Buildings.push_back(furnace);
	MenuItemSprite* furnaceItem = MenuItemSprite::create(
		furnace,
		furnace,
		furnace,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	furnaceItem->setName("furnace");
	furnaceItem->setTag(5);
	furnaceItem->setPosition(Vec2(125, 613));
	menu->addChild(furnaceItem);

	Building* winetable = WineTable::create();
	Vec_Buildings.push_back(winetable);
	MenuItemSprite* winetableItem = MenuItemSprite::create(
		winetable,
		winetable,
		winetable,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	winetableItem->setName("winetable");
	winetableItem->setTag(6);
	winetableItem->setPosition(Vec2(540, 470));
	menu->addChild(winetableItem);

	loadJsonData();

	m_storageroom = StorageRoom::create();
	MenuItemSprite* storageroomItem = MenuItemSprite::create(
		m_storageroom,
		m_storageroom,
		m_storageroom,
		CC_CALLBACK_1(HomeLayer::onStorageRoom, this));
	storageroomItem->setPosition(Vec2(105, 188));
	menu->addChild(storageroomItem);

	Building* fence = Fence::create();
	MenuItemSprite* fenceItem = MenuItemSprite::create(
		fence,
		fence,
		fence,
		CC_CALLBACK_1(HomeLayer::onFence, this));
	fenceItem->setPosition(Vec2(630, 48));
	menu->addChild(fenceItem);


	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		StorageRoom::add(MyPackage::vec_packages[i]);
	}
	MyPackage::takeoff();

	g_hero->setIsOut(false);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HomeLayer::onclick(Ref* pSender)
{
	Node* node = (Node*)pSender;
	std::string nodename = node->getName();
	int nodetag = node->getTag();

	Layer* layer = BuildingUILayer::create(Vec_Buildings[nodetag]);
	Director::getInstance()->getRunningScene()->addChild(layer);
}

void HomeLayer::loadJsonData()
{
	//建筑的JSON数据
	rapidjson::Document doc = ReadJsonFile("data/buildings.json");
	rapidjson::Value& allBuilds = doc["b"];
	for (unsigned int i = 0; i < Vec_Buildings.size(); i++)
	{
		rapidjson::Value& oneBuild = allBuilds[i];
		Vec_Buildings[i]->parseData(oneBuild);
	}
}

void HomeLayer::onStorageRoom(Ref* pSender)
{
	Layer* layer = StorageUILayer::create();
	Director::getInstance()->getRunningScene()->addChild(layer);
}

void HomeLayer::onFence(Ref* pSender)
{
	Layer* layer = OutDoor::create();
	Director::getInstance()->getRunningScene()->addChild(layer);
}