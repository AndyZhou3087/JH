#ifndef _BUYCOMFIRM_LAYER_H_
#define _BUYCOMFIRM_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class BuyComfirmLayer :public Layer
{
public:
	BuyComfirmLayer();
	~BuyComfirmLayer();

	bool init(int shopGoodsIndex);
	static BuyComfirmLayer* create(int shopGoodsIndex);

private:

	int m_goodsIndex;
private:
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

