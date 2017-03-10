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
private:
	void onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void loadJsonData();
	void updateUI(float dt);
private:
	std::vector<int> vec_resid;
	cocos2d::ui::ScrollView* scrollView;
	int pasttime;
};
#endif

