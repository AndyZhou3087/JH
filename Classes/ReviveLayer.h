#ifndef _REVIVE_LAYER_H_
#define _REVIVE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class ReviveLayer :public Layer
{
public:
	ReviveLayer();
	~ReviveLayer();

	virtual bool init();
	static ReviveLayer* create();
private:


private:
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onRevive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

