#ifndef _OUTDOOR_LAYER_H_
#define _OUTDOOR_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;


class OutDoor :public Layer
{
public:
	OutDoor();
	~OutDoor();

	virtual bool init();
	CREATE_FUNC(OutDoor);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onOut(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	Node* m_csbnode;
	cocos2d::ui::ScrollView * scrollview;
	void updata();
	void onStorageItem(cocos2d::Ref* pSender);
	void onPackageItem(cocos2d::Ref* pSender);
	void removeitem();
private:
	std::vector<PackageData*> allStorageData;
};
#endif

