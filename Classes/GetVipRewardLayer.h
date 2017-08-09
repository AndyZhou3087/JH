#ifndef _GETVIPREWARD_LAYER_H_
#define _GETVIPREWARD_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalData.h"
USING_NS_CC;

class GetVipRewardLayer :public Layer
{
public:
	GetVipRewardLayer();
	~GetVipRewardLayer();

	bool init();
	static GetVipRewardLayer* create();

private:
private:
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

