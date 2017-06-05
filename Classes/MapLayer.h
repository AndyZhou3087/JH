/********************************************************************
*地图界面
*********************************************************************/
#ifndef _MAP_LAYER_H_
#define _MAP_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class MapLayer :public Layer
{
public:
	MapLayer();
	~MapLayer();

	virtual bool init();

	CREATE_FUNC(MapLayer);

	/****************************
	角色头像移动的目的地
	*****************************/
	void showMoveToDest();

	/****************************
	更新显示解锁的地点
	*****************************/
	void updateUnlockChapter();

	/****************************
	显示解锁界面
	*****************************/
	void showUnlockLayer(float dt);

	/****************************
	更新主线任务的图标显示
	*****************************/
	void updataPlotMissionIcon();

	/****************************
	延迟显示新手引导
	*****************************/
	void delayShowMapNewerGuide(float dt);

	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);
private:

	/****************************
	点击地名
	*****************************/
	void onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	
	*****************************/
	void onShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void Arrive(float dt);
	void movefinish(float dt);

private:
	float m_distance;
	Vec2 m_destPos;
	std::string m_addrname;
	Sprite* m_herohead;
	bool ismoving;
	cocos2d::ui::Widget* m_mapbg;
	Sprite* m_smissionIcon;
	Sprite* m_dmissionIcon;
};
extern MapLayer* g_maplayer;
#endif

