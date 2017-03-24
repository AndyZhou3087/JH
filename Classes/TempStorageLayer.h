﻿#ifndef _TEMPSTORAGE_LAYER_H_
#define _TEMPSTORAGE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;
class TempStorageLayer :public Layer
{
public:
	TempStorageLayer();
	~TempStorageLayer();

	bool init(std::string addrname);

	static TempStorageLayer* create(std::string addrname);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updata();
	void onRewardItem(cocos2d::Ref* pSender);
	void onPackageItem(cocos2d::Ref* pSender);
	void loadTempData();
	void saveTempData();
	void removeitem();
private:
	std::vector<PackageData> tempResData;
	std::string m_addrname;
	cocos2d::ui::ScrollView* m_scrollView;

};
#endif

