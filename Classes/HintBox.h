
#ifndef __HINTBOX__
#define __HINTBOX__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class HintBox : public Layer
{
public:
    bool init(std::string text);
	static HintBox* create(std::string content);
	void removSelf();
private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif
