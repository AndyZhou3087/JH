﻿#ifndef _BEDUI_LAYER_H_
#define _BEDUI_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Building.h"
USING_NS_CC;

typedef enum
{
	BUILD,
	ACTION
}BACTIONTYPE;

class BuildingUILayer :public Layer
{
public:
	BuildingUILayer();
	~BuildingUILayer();

	virtual bool init(Building* build);

	static BuildingUILayer* create(Building* build);

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	Building* m_build;
	Node* buildnode;
	Node* m_csbnode;
	cocos2d::ui::LoadingBar* buildbar;
	cocos2d::ui::Button* buildbtn;
	cocos2d::ui::ScrollView* scrollview;

	std::vector<cocos2d::ui::LoadingBar*> vec_actionbar;
	std::vector<cocos2d::ui::Button*> vec_actionbtn;
	void onfinish(Ref* pSender, BACTIONTYPE type);
	void updataBuildRes();
	void loadActionUi();
	void updataActionRes();
private:
	std::vector<Node*> vec_actionItem;
};
#endif

