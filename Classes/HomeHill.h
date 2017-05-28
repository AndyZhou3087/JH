#ifndef _HOMEHILL_LAYER_H_
#define _HOMEHILL_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class HomeHill :public Layer
{
public:
	HomeHill();
	~HomeHill();

	virtual bool init();

	CREATE_FUNC(HomeHill);
	virtual void onEnterTransitionDidFinish();
	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);
private:
	void onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updateUI(float dt);

private:

	cocos2d::ui::ScrollView* scrollView;
	cocos2d::ui::Widget* m_backbtn;
};
#endif

