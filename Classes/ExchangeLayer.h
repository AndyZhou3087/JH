#ifndef _EXCHANGE_LAYER_H_
#define _EXCHANGE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;
class ExchangeLayer :public Layer
{
public:
	ExchangeLayer();
	~ExchangeLayer();

	bool init(std::string npcid);
	virtual void onExit();
	virtual void onEnterTransitionDidFinish();
	static ExchangeLayer* create(std::string npcid);
	void updataMyGoodsUI();
	void updataNpcGoodsUI();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onExg(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updata();
	void onNpcGoodsItem(cocos2d::Ref* pSender);
	void onMyGoodsItem(cocos2d::Ref* pSender);
	void updateMyGoods(PackageData data, int type);//type:0-是我的物品，1-npc的物品
	void updateNpcGoods(PackageData data, int type);//type:0-是我的物品，1-npc的物品

	void checkValue();
	void randExchgRes(std::vector<std::string> &vec_exchgres);
private:
	cocos2d::ui::ScrollView* m_npcGoodsSrollView;
	cocos2d::ui::ScrollView* m_myGoodsSrollView;
	cocos2d::ui::Widget* m_exgbtn;
	cocos2d::ui::Text* m_npcWordLbl;
	int lastMyGoodsSrollViewHeight;
	int lastNpcGoodsSrollViewHeight;
	std::string m_npcid;
	std::vector<PackageData> npcGoodsData;
	std::vector<PackageData> myGoodsData;
	std::vector<PackageData> npcExgData;
	std::vector<PackageData> myExgData;
	bool isExgOk;
};
#endif

