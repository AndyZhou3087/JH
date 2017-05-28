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
	void showMoveToDest();
	void updateUnlockChapter();

	void showUnlockLayer(float dt);
	void updataPlotMissionIcon();
	void delayShowMapNewerGuide(float dt);
	void showNewerGuide(int step);
private:
	void onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
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

