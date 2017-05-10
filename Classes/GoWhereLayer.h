﻿
/********************************************************************
*地点显示界面
*********************************************************************/
#ifndef _GOWHERE_LAYER_H_
#define _GOWHERE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

/****************************
地点操作类型
*****************************/
typedef enum
{
	GOWHERE = 0,//准备去
	ARRIVE//到达
}WHERELAYER_TYPE;

class GoWhereLayer :public Layer
{
public:
	GoWhereLayer();
	~GoWhereLayer();

	/****************************
	初始化
	@param 地点id
	@param 地点操作类型
	@param 目的地距离，像素单位
	*****************************/
	bool init(std::string addrname, WHERELAYER_TYPE type, float distance = 0.0f);
	static GoWhereLayer* create(std::string addrid, WHERELAYER_TYPE type, float distance = 0.0f);
private:

	/****************************
	点击“算了”按钮回调
	*****************************/
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“出发”按钮回调
	*****************************/
	void onGO(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“物品存放”按钮回调
	*****************************/
	void onST(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击“进入”按钮回调
	*****************************/
	void onComeIn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
	std::string m_addrstr;//地点ID
	WHERELAYER_TYPE m_type;
	static float m_distance;
};
#endif

