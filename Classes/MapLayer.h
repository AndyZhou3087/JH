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

	/****************************
	移动到目的地的时间
	*****************************/
	float moveToDestTime(float distance);

	/****************************
	遇到山贼暂停,暂停赶路
	*****************************/
	void heroPauseMoving();

	/****************************
	恢复赶路
	*****************************/
	void heroResumeMoving();

	/****************************
	恢复赶路
	*****************************/
	void showRobberFight(float dt);

private:

	/****************************
	点击地名
	*****************************/
	void onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击商城按钮回调
	*****************************/
	void onShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	到达目的地
	*****************************/
	void Arrive();

	/****************************
	地图移动
	*****************************/
	void mapMoveTo(Vec2 pos);

private:
	float m_distance;//达到目的地的像素距离
	Vec2 m_destPos;//目的地像素坐标
	std::string m_addrname;//目的地名称
	Sprite* m_herohead;//角色头像
	bool ismoving;//是否正在移动
	cocos2d::ui::Widget* m_mapbg;//整个地图
	Sprite* m_smissionIcon;//发任务的图标
	Sprite* m_dmissionIcon;//接任务的图标
	cocos2d::ui::ScrollView* m_mapscroll;//地图
};
extern MapLayer* g_maplayer;//全局地图层
#endif

