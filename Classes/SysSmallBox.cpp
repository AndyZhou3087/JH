#include "SysSmallBox.h"
#include "CommonFuncs.h"
SysSmallBox::SysSmallBox()
{
}


SysSmallBox::~SysSmallBox()
{
}

bool SysSmallBox::init(std::string imagepath, std::string title, std::string text)
{
	Node* csbnode = CSLoader::createNode("sysSmallLayer.csb");
	this->addChild(csbnode);
	image = (cocos2d::ui::ImageView*)csbnode->getChildByName("image");
	titleTxt = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	textTxt = (cocos2d::ui::Text*) csbnode->getChildByName("text");

	image->loadTexture(imagepath, cocos2d::ui::TextureResType::PLIST);
	image->setContentSize(Sprite::createWithSpriteFrameName(imagepath)->getContentSize());
	image->setScale(1.5f);
	titleTxt->setString(CommonFuncs::gbk2utf(title.c_str()));
	textTxt->setString(CommonFuncs::gbk2utf(text.c_str()));

	return true;
}

SysSmallBox* SysSmallBox::create(std::string imagepath, std::string title, std::string text)
{
	SysSmallBox *pRet = new SysSmallBox();
	if (pRet && pRet->init(imagepath, title, text))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}
