/********************************************************************
* 建筑物UI界面
*********************************************************************/
#ifndef _BUIDINGUI_LAYER_H_
#define _BUIDINGUI_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Building.h"
USING_NS_CC;

typedef enum
{
	BUILD,//建造或者升级
	ACTION//每个建筑物自己的操作，eg:床：睡觉；药箱：制药...
}BACTIONTYPE;

class BuildingUILayer :public Layer
{
public:
	BuildingUILayer();
	~BuildingUILayer();

	virtual bool init(Building* build);
	virtual void onEnterTransitionDidFinish();

	static BuildingUILayer* create(Building* build);

private:

	/****************************
	点击返回按钮回调
	*****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击建筑物自己的操作按钮回调
	*****************************/
	void onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击建筑物图标回调，显示详细信息
	*****************************/
	void onBuidingDetails(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击资源回调，显示详细信息
	*****************************/
	void onResDetails(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	更新建筑物等级资源UI
	****************************/
	void updataBuildRes();

	/****************************
	加载建筑物自己操作的UI
	****************************/
	void loadActionUi();

	/****************************
	更新建筑物自己操作的UI
	****************************/
	void updataActionRes();

	/****************************
	更新建筑物自己操作的UI
	****************************/
	void showNewerGuide(float dt);

	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);
private:
	Building* m_build;
	Node* buildnode;//每个ITEM NODE
	Node* m_csbnode;//UI NODE
	cocos2d::ui::LoadingBar* buildbar;//建造进度条控件
	cocos2d::ui::Button* buildbtn;//建造按钮控件
	cocos2d::ui::ScrollView* scrollview;//操作滚动列表控件

	std::vector<cocos2d::ui::LoadingBar*> vec_actionbar;//建筑物自己的操作 进度条控件
	std::vector<cocos2d::ui::Button*> vec_actionbtn;//建造进度条控件
	void onfinish(Ref* pSender, BACTIONTYPE type);//建造，建筑物自己的操作 完成

	void showFinishHintText(std::string path);//显示完成后的提示文字
	void finishAnim(Ref* pSender, Node* node);//建造，建筑物自己的操作 完成后的提示动画
	std::vector<Node*> vec_actionItem;//建筑物自己操作 item node
};
#endif

