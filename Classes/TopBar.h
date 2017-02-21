#ifndef _TOPBAR_H_
#define _TOPBAR_H_
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

class TopBar:public Node
{
public:
	TopBar();
	~TopBar();

	virtual bool init();

	CREATE_FUNC(TopBar);
private:
	void onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

