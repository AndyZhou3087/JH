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
	m_width = width;
	m_height = height;

	m_containerLayer = Layer::create();
	m_containerLayer->setAnchorPoint(Vec2(0, 0));

	scrollView = ScrollView::create();
	scrollView->setDirection(ScrollView::Direction::VERTICAL);
	scrollView->setPosition(0, 0);
	scrollView->setViewSize(Size(width, height));
	scrollView->setContentSize(Size(m_width, m_height));
	scrollView->setDelegate(this);

	scrollView->setBounceable(true);
	scrollView->setContainer(m_containerLayer);
	this->addChild(scrollView);

	return true;
}

void UIScroll::addEventText(std::string text, int fontsize, Color3B color3b)
{
	Label* textlbl = Label::createWithTTF(text, "fonts/STXINGKA.TTF", fontsize);
	textlbl->setWidth(m_width);
	textlbl->setColor(color3b);
	int curlblheight = textlbl->getContentSize().height;

	Vector<Node*> vec_tlbl = scrollView->getContainer()->getChildren();
	for (int i = 0; i < vec_tlbl.size(); i++)
	{
		Node* tlbl = vec_tlbl.at(i);
		tlbl->setPositionY(tlbl->getPositionY() + curlblheight + 8);
	}
	int viewHeight = scrollView->getViewSize().height;
	scrollView->setContentSize(Size(m_width, viewHeight + curlblheight));
	m_containerLayer->addChild(textlbl);

	//if ()
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
