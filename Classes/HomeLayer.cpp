#include "HomeLayer.h"
#include "Bed.h"
#include "Furnace.h"
#include "Forgingtable.h"
#include "CookTable.h"
#include "MedicineKit.h"
#include "WineTable.h"
#include "WineMaker.h"
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

	Node* bed = Bed::create();
	MenuItemSprite* bedItem = MenuItemSprite::create(
		bed,
		bed,
		bed,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	bedItem->setName("bed");
	bedItem->setPosition(Vec2(160, 700));
	Menu* menu = Menu::create();
	menu->addChild(bedItem);
	menu->setPosition(Vec2(0, 0));
	csbnode->addChild(menu);

	Furnace* furnace = Furnace::create();
	MenuItemSprite* furnaceItem = MenuItemSprite::create(
		furnace,
		furnace,
		furnace,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	furnaceItem->setName("furnace");
	furnaceItem->setPosition(Vec2(125, 613));
	menu->addChild(furnaceItem);

	Forgingtable* forgingtable = Forgingtable::create();
	MenuItemSprite* forgingtableItem = MenuItemSprite::create(
		forgingtable,
		forgingtable,
		forgingtable,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	forgingtableItem->setName("forgingtable");
	forgingtableItem->setPosition(Vec2(195, 410));
	menu->addChild(forgingtableItem);

	CookTable* cooktable = CookTable::create();
	MenuItemSprite* cooktableItem = MenuItemSprite::create(
		cooktable,
		cooktable,
		cooktable,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	cooktableItem->setName("cooktable");
	cooktableItem->setPosition(Vec2(580, 786));
	menu->addChild(cooktableItem);

	MedicineKit* medicinekit = MedicineKit::create();
	MenuItemSprite* medicinekitItem = MenuItemSprite::create(
		medicinekit,
		medicinekit,
		medicinekit,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	medicinekitItem->setName("medicinekit");
	medicinekitItem->setPosition(Vec2(678, 625));
	menu->addChild(medicinekitItem);

	WineTable* winetable = WineTable::create();
	MenuItemSprite* winetableItem = MenuItemSprite::create(
		winetable,
		winetable,
		winetable,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	winetableItem->setName("winetable");
	winetableItem->setPosition(Vec2(540, 470));
	menu->addChild(winetableItem);

	WineMaker* winemaker = WineMaker::create();
	MenuItemSprite* winemakerItem = MenuItemSprite::create(
		winemaker,
		winemaker,
		winemaker,
		CC_CALLBACK_1(HomeLayer::onclick, this));
	winemakerItem->setName("winemaker");
	winemakerItem->setPosition(Vec2(445, 800));
	menu->addChild(winemakerItem);

	return true;
}

void HomeLayer::onclick(Ref* pSender)
{
	Node* node = (Node*)pSender;
	std::string nodename = node->getName();
	if (nodename.compare("bed") == 0)
	{

	}
}
