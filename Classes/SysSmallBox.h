#ifndef _SYSSMALL_BOX_H_
#define _SYSSMALL_BOX_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class SysSmallBox :public Layer
{
public:
	SysSmallBox();
	~SysSmallBox();

	virtual bool init(std::string imagepath, std::string title, std::string text);
	static SysSmallBox* create(std::string imagepath, std::string title, std::string text);
private:
	cocos2d::ui::ImageView* image;
	cocos2d::ui::Text* titleTxt;
	cocos2d::ui::Text* textTxt;
};
#endif

