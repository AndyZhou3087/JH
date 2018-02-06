#ifndef _RECHARGE_LAYER_H_
#define _RECHARGE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
#include "ServerDataSwap.h"
USING_NS_CC;

class RechargeLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	RechargeLayer();
	~RechargeLayer();

	bool init();
	static RechargeLayer* create();
	static int getRechargeMaxAmount();
private:
	
	cocos2d::ui::Button* getbtn;
	cocos2d::ui::Text* datelbl;
	cocos2d::ui::Text* progresstext;
	cocos2d::ui::LoadingBar* progressbar;
	std::map<int, std::vector<std::string>> map_recharge;
	int maxamount;
	Node* m_csbnode;

	std::vector<std::string> vec_myawrd;
	int canGetHightAmount;
	cocos2d::ui::Widget* descbg;
private:
	void showdata();
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBoxClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBGClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);

	void splitRechargeAward();

	void showDescAward(int amount);
	
};
#endif

