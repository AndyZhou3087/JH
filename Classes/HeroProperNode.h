/********************************************************************
*角色装备栏
*********************************************************************/
#ifndef _HEROPROPER_H_
#define _HEROPROPER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Hero.h"
USING_NS_CC;


class HeroProperNode :public Node
{
public:
	HeroProperNode();
	~HeroProperNode();

	virtual bool init();
	virtual void onExit();
	CREATE_FUNC(HeroProperNode);

	/****************************
	刷新map_carryData 列表
	*****************************/
	void refreshCarryData();
private:

	/****************************
	点击“确定”按钮回调
	*****************************/
	void onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	点击每个物品栏回调
	*****************************/
	void onImageClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	弹出装备选择框
	@param 选中的类型
	*****************************/
	void showSelectFrame(HeroAtrType index);

	//装备的滚动控件
	cocos2d::ui::ScrollView* m_scrollView;

	/****************************
	选中装备装上或卸下
	@param 点击的item
	*****************************/
	void onItem(Ref* pSender);

	/****************************
	选择另外一种类型的移除掉之前的item
	*****************************/
	void removeitem();

	/****************************
	将装备添加到map_carryData 列表
	@param 装备类型
	*****************************/
	void addCarryData(HeroAtrType index);

	/****************************
	保存数据
	*****************************/
	void saveData();

	/****************************
	更新背包栏数据
	*****************************/
	void updataMyPackageUI();
private:
	Node* csbroot;
	cocos2d::ui::ImageView* propeImages[8];//8种装备类型
	cocos2d::ui::Text* lvtext[8];//8种装备等级或者耐久度
	cocos2d::ui::Widget* heroselectbg;//选择装备背景
	cocos2d::ui::Widget* heroppoint;//装备背景上的三角形
	cocos2d::ui::Text* title;//标题
	int lastclickindex;//上次选中装备栏类型index
	bool isout;//是否在外面
	std::map<HeroAtrType, std::vector<PackageData>> map_carryData;//装备栏数据
	Sprite* m_select;//选中标识
	PackageData* m_lastSelectedData;//上次选中中的装备数据
	cocos2d::EventListenerTouchOneByOne* m_listener;
};
#endif

