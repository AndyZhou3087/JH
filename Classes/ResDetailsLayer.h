
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
	bool init(PackageData* pdata);
	static ResDetailsLayer* create(PackageData* pdata);
	void removSelf();
private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
	PackageData* m_packageData;
};

#endif
