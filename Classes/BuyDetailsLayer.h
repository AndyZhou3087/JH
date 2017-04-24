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

	virtual bool init(std::string imagepath, std::string name, std::string desc, int price);
	static BuyDetailsLayer* create(std::string imagepath, std::string name, std::string desc, int price);
private:
	cocos2d::ui::ImageView* image;
	cocos2d::ui::Text* nameTxt;
	cocos2d::ui::Text* descTxt;
	cocos2d::ui::Text* priceTxt;

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

