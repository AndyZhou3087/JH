/****************************
角色购买界面
****************************/
#ifndef _BUYDETAILS_LAYER_H_
#define _BUYDETAILS_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class BuyDetailsLayer :public Layer
{
public:
	BuyDetailsLayer();
	~BuyDetailsLayer();

	/****************************
	初始化
	@param imagepath 角色头像
	@param name 角色名称
	@param desc 角色描述
	@param price 价格
	****************************/
	virtual bool init(std::string imagepath, std::string name, std::string desc, int price);
	static BuyDetailsLayer* create(std::string imagepath, std::string name, std::string desc, int price);
private:
	cocos2d::ui::ImageView* image;//头像 image
	cocos2d::ui::Text* nameTxt;//角色名称控件
	cocos2d::ui::Text* descTxt;//角色描述控件
	cocos2d::ui::Text* priceTxt;//角色价格控件

private:
	/****************************
	点击返回按钮回调
	*****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击购买按钮回调
	*****************************/
	void onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

