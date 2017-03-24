#ifndef _HEROSTATEUI_LAYER_H_
#define _HEROSTATEUI_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;


class HeroStateUILayer :public Layer
{
public:
	HeroStateUILayer();
	~HeroStateUILayer();

	virtual bool init();
	CREATE_FUNC(HeroStateUILayer);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	Node* m_csbnode;
private:
};
#endif

