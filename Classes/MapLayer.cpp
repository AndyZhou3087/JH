#include "MapLayer.h"
#include "json.h"
#include "HomeLayer.h"
#include "HomeHill.h"

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
	float offsetx = 0.0f;
	float offsety = 0.0f;
	Size scollviewsize = mapscroll->getContentSize();
	Vec2 pos = mapbg->getChildren().at(0)->getPosition();

	if (pos.x > scollviewsize.width / 2)
		offsetx = pos.x - scollviewsize.width / 2;
	if (pos.y > scollviewsize.height / 2)
		offsety = pos.y - scollviewsize.height / 2;

	mapscroll->setInnerContainerPosition(Vec2(-offsetx, -offsety));
	return true;
}

void MapLayer::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int nodetag = node->getTag();

		if (nodetag == 1001)
		{
			this->getParent()->addChild(HomeLayer::create(), 1, "homelayer");
			this->removeFromParentAndCleanup(true);
		}
		else
		{
			this->getParent()->addChild(HomeHill::create(), 3);	
		}
	}
}