#include "MapLayer.h"
#include "json.h"

MapLayer::MapLayer()
{
}


MapLayer::~MapLayer()
{
}

bool MapLayer::init()
{
	Node* csbnode = CSLoader::createNode("mapLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::ScrollView* mapscroll = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");

	cocos2d::ui::Widget* mapbg = (cocos2d::ui::Widget*)mapscroll->getChildByName("mapbg");
	int mapnamecount = mapbg->getChildrenCount();
	for (int i = 0; i < mapnamecount; i++)
	{
		int ctag = mapbg->getChildren().at(i)->getTag();

		cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)mapbg->getChildren().at(i);
		mapname->setTag(ctag);
		mapname->addTouchEventListener(CC_CALLBACK_2(MapLayer::onclick, this));

	}
	Vec2 pos = mapbg->getChildren().at(0)->getPosition();
	mapscroll->setInnerContainerPosition(Vec2(pos.x - 360, pos.y));
	return true;
}

void MapLayer::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

	}
}