#ifndef _UISCROLL_H_
#define _UISCROLL_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
USING_NS_CC_EXT;
class UIScroll :public Node, public ScrollViewDelegate
{
public:
	UIScroll();
	~UIScroll();

	bool init(float width, float height);

	static UIScroll* UIScroll::create(float width, float height);
	void addEventText(std::string text, int fontsize = 16, Color3B color3b = Color3B(0,0,0));

private:
	float m_width;
	float m_height;
	Layer* m_containerLayer;
	ScrollView *scrollView;
	void scrollViewDidScroll(ScrollView* view);

	void onTouchEnded(Touch*touch, Event *unused_event);
	bool onTouchBegan(Touch * touch, Event *unused_event);//¿ªÊ¼×´Ì¬
	void onTouchMoved(Touch *touch, Event *unused_event);//ÒÆ¶¯×´Ì¬
};
#endif

