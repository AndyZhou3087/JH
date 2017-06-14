
/****************************************************************
商城物品类

****************************************************************/
#ifndef _GOODSITIME_H_
#define _GOODSITIME_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

/****************************
商城物品数据结构
*****************************/
typedef struct
{
	std::string icon;//显示ICON
	std::string name;//显示名称
	int count;//出售个数
	int price;//价格
	std::string desc;//描述
	std::vector<std::string> vec_res;//购买成功后获得的物品（礼包类）
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
	bool isDraging;
private:

	/****************************
	点击购买按钮回调
	*****************************/
	void onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

