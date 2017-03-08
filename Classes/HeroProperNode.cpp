#include "HeroProperNode.h"
#include "json.h"
#include "CommonFuncs.h"
#include "Const.h"

const std::string name[] = { "外功", "内功", "武器", "防具", "工具", "工具", "工具", "坐骑"};

HeroProperNode::HeroProperNode()
{
	lastclickindex = -1;
}


HeroProperNode::~HeroProperNode()
{
}

bool HeroProperNode::init()
{
	csbroot = CSLoader::createNode("heroAttribNode.csb");
	this->addChild(csbroot);
	for (int i = 0; i < 8; i++)
	{
		std::string str = StringUtils::format("Image_%d", i);
		propeImages[i] = (cocos2d::ui::ImageView*)csbroot->getChildByName(str);
		str = StringUtils::format("box_%d", i);

		cocos2d::ui::Button* imgbtn = (cocos2d::ui::Button*)csbroot->getChildByName(str);
		imgbtn->setTag(i);
		imgbtn->addTouchEventListener(CC_CALLBACK_2(HeroProperNode::onImageClick, this));
	}

	heroselectbg = (cocos2d::ui::Widget*)csbroot->getChildByName("heroselectbg");
	heroppoint = (cocos2d::ui::Widget*)csbroot->getChildByName("heroppoint");

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)heroselectbg->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(HeroProperNode::onOK, this));

	title = (cocos2d::ui::Text*)heroselectbg->getChildByName("title");
	return true;
}

void HeroProperNode::onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		lastclickindex = -1;
		heroselectbg->setVisible(false);
		heroppoint->setVisible(false);
	}
}

void HeroProperNode::onImageClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int tag = node->getTag();

		if (lastclickindex == tag)
			return;

		lastclickindex = tag;
		title->setString(CommonFuncs::gbk2utf(name[tag].c_str()));
		heroppoint->setVisible(true);
		heroselectbg->setVisible(true);
		heroppoint->setPosition(Vec2(propeImages[tag]->getPositionX(), propeImages[tag]->getPositionY() - propeImages[tag]->getContentSize().height / 2 - 5));

		heroselectbg->setPositionY(heroppoint->getPositionY() - heroppoint->getContentSize().height + 3);
	}
}