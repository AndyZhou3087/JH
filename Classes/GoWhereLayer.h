#ifndef _GOWHERE_LAYER_H_
#define _GOWHERE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

typedef enum
{
	GOWHERE = 0,
	ARRIVE
}WHERELAYER_TYPE;

class GoWhereLayer :public Layer
{
public:
	GoWhereLayer();
	~GoWhereLayer();

	bool init(std::string addrname, WHERELAYER_TYPE type, float distance = 0.0f);
	static GoWhereLayer* create(std::string addrname, WHERELAYER_TYPE type, float distance = 0.0f);
private:
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onGO(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onST(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onComeIn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
	std::string m_addrstr;
	WHERELAYER_TYPE m_type;
	static float m_distance;
};
#endif

