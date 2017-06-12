/********************************************************************
* 闭关结束结算界面
*********************************************************************/
#ifndef _EXERCISEDONE_LAYER_H_
#define _EXERCISEDONE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class ExerciseDoneLayer :public Layer
{
public:
	ExerciseDoneLayer();
	~ExerciseDoneLayer();

	virtual bool init();
	CREATE_FUNC(ExerciseDoneLayer);
private:
	void showText(float dt);
	void removeself(float dt);

private:
	cocos2d::ui::Widget* m_unlockbg;
	cocos2d::ui::Text* m_title;
	cocos2d::ui::Text* m_herotext;
	cocos2d::ui::Text* m_wgtext;
	cocos2d::ui::Text* m_ngtext;
};
#endif

