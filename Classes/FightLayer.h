#ifndef _FIGHT_LAYER_H_
#define _FIGHT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UIScroll.h"
USING_NS_CC;
class FightLayer :public Layer
{
public:
	FightLayer();
	~FightLayer();

	bool init(std::string addrname, std::string npcname);

	static FightLayer* create(std::string addrname, std::string npcname);
private:
	void onEscape (cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
	UIScroll* m_fihgtScorll;
	cocos2d::ui::Button* m_escapebtn;
};
#endif

