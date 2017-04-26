#ifndef _GOODSITIME_H_
#define _GOODSITIME_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

typedef struct
{
	std::string icon;
	std::string name;
	int count;
	int price;
	std::string desc;
	std::vector<std::string> vec_res;
}GoodsData;

class GoodsItem :public Sprite
{
public:
	GoodsItem();
	~GoodsItem();

	virtual bool init(GoodsData* gdata);
	static GoodsItem* create(GoodsData* gdata);
private:
	cocos2d::ui::ImageView* icon;
	cocos2d::ui::Text* nameTxt;
	cocos2d::ui::Text* descTxt;
	cocos2d::ui::Text* priceTxt;

private:
	void onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

