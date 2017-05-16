
#ifndef __RES_DETAILS_LAYER__
#define __RES_DETAILS_LAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;

class ResDetailsLayer : public Layer
{
public:
	ResDetailsLayer();
	~ResDetailsLayer();
	bool init(PackageData* pdata);
	static ResDetailsLayer* create(PackageData* pdata);
	static ResDetailsLayer* createByResId(std::string resid);
	void removSelf();

	static int whereClick;
private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onUse(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void recoveInjuryValue(int addwvalue, int addnvalue);
	void recoveHungerValue(int addvalue);
private:
	PackageData* m_packageData;
	cocos2d::ui::Text* uselbl;
};

#endif
