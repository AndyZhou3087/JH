#ifndef _NPC_LAYER_H_
#define _NPC_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class NpcLayer :public Layer
{
public:
	NpcLayer();
	~NpcLayer();

	bool init(std::string addrname);
	static NpcLayer* create(std::string addrname);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemTalk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemExchange(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
	std::string m_addrstr;
};
#endif

