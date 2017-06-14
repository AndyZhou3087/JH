#ifndef _SHOP_LAYER_H_
#define _SHOP_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GoodsItem.h"
USING_NS_CC;

typedef enum
{
	PAY_SUCC = 0,
	PAY_FAIL
}PYARET;

class ShopLayer :public Layer
{
public:
	ShopLayer();
	~ShopLayer();

	bool init();
	static ShopLayer* create();
	static void beginPay(int index);
	static void setMessage(PYARET ret);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	static void loadShopData();
	static void addBuyGoods();
private:
	static std::vector<GoodsData> vec_goods;
	cocos2d::ui::ScrollView* m_scrollview;
	static int payindex;
};
#endif

