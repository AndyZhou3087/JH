#ifndef _BEDUI_LAYER_H_
#define _BEDUI_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Building.h"
USING_NS_CC;
typedef struct
{
	char icon[20];
	std::vector<int> res;
	int blv;
	char actext[32];
	int actime;
}BuildActionData;

typedef enum
{
	BUILD,
	ACTION
}BACTIONTYPE;

class BedUILayer :public Layer
{
public:
	BedUILayer();
	~BedUILayer();

	virtual bool init(Building* build);

	static BedUILayer* create(Building* build);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	Building* m_buildData;
	Node* buildnode;
	Node* m_csbnode;
	cocos2d::ui::LoadingBar* buildbar;
	cocos2d::ui::Button* buildbtn;
	cocos2d::ui::ScrollView* scrollview;

	std::vector<cocos2d::ui::LoadingBar*> vec_actionbar;
	std::vector<cocos2d::ui::Button*> vec_actionbtn;
	void onfinish(Ref* pSender, BACTIONTYPE type);
	void updataBuildRes();
	void loadActionUi();
	void parseBuildActionJSon();
private:
	std::map<std::string, std::vector<BuildActionData>> map_buidACData;
};
#endif

