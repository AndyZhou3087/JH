#ifndef _ACTIONGET_LAYER_H_
#define _ACTIONGET_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class ActionGetLayer :public Layer
{
public:
	ActionGetLayer();
	~ActionGetLayer();

	bool init(std::vector<int> vec_id, int type, int actype);

	static ActionGetLayer* create(std::vector<int> vec_id, int type, int actype);
private:
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updata();
	void onRewardItem(cocos2d::Ref* pSender);
private:
	std::vector<int> rewardRes_ids;
};
#endif

