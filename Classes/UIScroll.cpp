#include "UIScroll.h"

UIScroll::UIScroll()
{
}


UIScroll::~UIScroll()
{
}

UIScroll* UIScroll::create(float width, float height)
{
	UIScroll *pRet = new UIScroll();
	if (pRet && pRet->init(width, height))
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

bool UIScroll::init(float width, float height)
{
	scrollView = ScrollView::create();
	scrollView->setDirection(ScrollView::Direction::VERTICAL);
	scrollView->setPosition(0, 0);
	scrollView->setViewSize(Size(width, height));
	//scrollView->setContentSize(Size(visibleSize.width, visibleSize.height * (themecount)));
	scrollView->setColor(Color3B(255, 150, 100));
	scrollView->setDelegate(this);

	scrollView->setBounceable(true);
	this->addChild(scrollView);
	return true;
}

void UIScroll::addEventText(std::string text, int fontsize, Color3B color3b)
{
	Label* textlbl = Label::createWithTTF(text, "fonts/STXINGKA.TTF", fontsize);
	textlbl->setColor(color3b);
	scrollView->addChild(textlbl);
	int lastScrollViewH = scrollView->
	scrollView->setContentSize(Size(scrollView->getContentSize().width, scrollView->getContentSize().height + textlbl->getContentSize().height));
}

bool UIScroll::onTouchBegan(Touch * touch, Event *unused_event)
{
	return true;

}

void UIScroll::onTouchMoved(Touch*touch, Event*unused_event)
{

}

void UIScroll::onTouchEnded(Touch*touch, Event *unused_event)
{

}


void UIScroll::scrollViewDidScroll(ScrollView* view)
{

}
