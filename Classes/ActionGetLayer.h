#ifndef _ACTIONGET_LAYER_H_
#define _ACTIONGET_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;
class ActionGetLayer :public Layer
{
public:
	ActionGetLayer();
	~ActionGetLayer();

	bool init(int rid, std::vector<int> res_ids, int type, int actype);
	virtual void onExit();
	static ActionGetLayer* create(int rid, std::vector<int> res_ids, int type, int actype);
private:
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updata();
	void onRewardItem(cocos2d::Ref* pSender);
	void onPackageItem(cocos2d::Ref* pSender);
	void doAction();
	void removeitem();
	void saveTempData();
private:
	int mrid;
	std::vector<int> rewardids;
	std::vector<PackageData> getResData;
	int mtype;
};
#endif

