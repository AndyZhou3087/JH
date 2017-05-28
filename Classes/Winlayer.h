#ifndef _WIN_LAYER_H_
#define _WIN_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;
class Winlayer :public Layer
{
public:
	Winlayer();
	~Winlayer();

	bool init(std::string addr, std::string npcid);
	virtual void onExit();
	virtual void onEnterTransitionDidFinish();
	static Winlayer* create(std::string addr, std::string npcid);
	void updataMyPackageUI();
	void updataRewardUI();
	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updata();
	void onRewardItem(cocos2d::Ref* pSender);
	void onPackageItem(cocos2d::Ref* pSender);
	void saveTempData();
	void loadTempData();
	void updataLV();
	void showLvUpText();
private:
	std::string m_npcid;
	std::string m_addrid;
	std::vector<PackageData> getRewardData;
	std::vector<PackageData> tempResData;
	cocos2d::ui::Text* explbl;
	cocos2d::ui::Text* gfexplbl;
	cocos2d::ui::Widget* m_backbtn;
	cocos2d::ui::Button* m_getallbtn;
};
#endif

