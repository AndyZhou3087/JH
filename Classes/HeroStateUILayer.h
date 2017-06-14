/********************************************************************
*角色装备属性层
*********************************************************************/
#ifndef _HEROSTATEUI_LAYER_H_
#define _HEROSTATEUI_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;


class HeroStateUILayer :public Layer
{
public:
	HeroStateUILayer();
	~HeroStateUILayer();

	virtual bool init();
	virtual void onExit();
	CREATE_FUNC(HeroStateUILayer);

	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);
private:
	/****************************
	点击“返回”按钮回调
	*****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“暂停”按钮回调
	*****************************/
	void onPause(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	Node* m_csbnode;
	//13个属性值显示控件
	cocos2d::ui::Text* herostatus[13];
private:
	/****************************
	定时更新13个属性值
	*****************************/
	void updateStatus(float dt);
};
#endif

