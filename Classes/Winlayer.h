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
	static Winlayer* create(std::string addr, std::string npcid);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updata();
	void onRewardItem(cocos2d::Ref* pSender);
	void onPackageItem(cocos2d::Ref* pSender);
	void removeitem();
	void saveTempData();
	int static systime();
	void updataLV();
	bool checkifHasGF(std::string gfid);
private:
	std::string m_npcid;
	std::vector<PackageData> getRewardData;
};
#endif

