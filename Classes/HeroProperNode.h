#ifndef _HEROPROPER_H_
#define _HEROPROPER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;


class HeroProperNode :public Node
{
public:
	HeroProperNode();
	~HeroProperNode();

	virtual bool init();
	CREATE_FUNC(HeroProperNode);
private:
	void onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onImageClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	cocos2d::ui::ScrollView* scrollview;
private:
	Node* csbroot;
	cocos2d::ui::ImageView* propeImages[8];
	cocos2d::ui::Widget* heroselectbg;
	cocos2d::ui::Widget* heroppoint;
	cocos2d::ui::Text* title;
	int lastclickindex;
};
#endif

