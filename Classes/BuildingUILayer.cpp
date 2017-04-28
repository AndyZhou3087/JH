#include "BuildingUILayer.h"
#include "json.h"
#include "Bed.h"
#include "MyActionProgressTimer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "Const.h"
#include "StorageRoom.h"
#include "HintBox.h"
#include "GlobalData.h"

BuildingUILayer::BuildingUILayer()
{

}


BuildingUILayer::~BuildingUILayer()
{
}


BuildingUILayer* BuildingUILayer::create(Building* build)
{
	BuildingUILayer *pRet = new BuildingUILayer();
	if (pRet && pRet->init(build))
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

bool BuildingUILayer::init(Building* build)
{
	m_build = build;
	m_csbnode = CSLoader::createNode("buidingUiLayer.csb");
	m_csbnode->setPosition(Vec2(0, -90));
	this->addChild(m_csbnode);

	cocos2d::ui::Text* title = (cocos2d::ui::Text*)m_csbnode->getChildByName("title");
	title->setString(m_build->data.cname);

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onBack, this));
	scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");
	if (m_build->data.level <= 0)
		scrollview->setVisible(false);

	buildnode = CSLoader::createNode("actionNode.csb");
	buildnode->setPosition(Vec2(360, 860));
	m_csbnode->addChild(buildnode);

	buildbtn = (cocos2d::ui::Button*)buildnode->getChildByName("item")->getChildByName("actionbtn");
	buildbtn->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onAction, this));
	buildbtn->setTag(BUILD);

	cocos2d::ui::ImageView* buildicon = (cocos2d::ui::ImageView*)buildnode->getChildByName("item")->getChildByName("box")->getChildByName("icon");
	std::string iconstr = StringUtils::format("ui/s%s.png", m_build->data.name);
	buildicon->loadTexture(iconstr, cocos2d::ui::TextureResType::PLIST);
	buildicon->setContentSize(Sprite::createWithSpriteFrameName(iconstr)->getContentSize());

	buildbar = (cocos2d::ui::LoadingBar*)buildnode->getChildByName("item")->getChildByName("loadingbar");

	updataBuildRes();

	loadActionUi();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

void BuildingUILayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void BuildingUILayer::loadActionUi()
{
	std::string name = m_build->data.name;

	int size = GlobalData::map_buidACData[name].size();

	int itemheight = 120;
	int scrollinnerheight = size * itemheight;

	int scorellheight = scrollview->getContentSize().height;
	if (scrollinnerheight < scorellheight)
		scrollinnerheight = scorellheight;
	scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, scrollinnerheight));

	if (vec_actionItem.size() <= 0)
	{
		for (int i = 0; i < size; i++)
		{
			Node *acnode = CSLoader::createNode("actionNode.csb");
			acnode->setPosition(Vec2(scrollview->getContentSize().width / 2, scrollinnerheight - itemheight / 2 - i * itemheight));
			scrollview->addChild(acnode);
			vec_actionItem.push_back(acnode);
		}
	}

	for (int i = 0; i < size; i++)
	{

		cocos2d::ui::Widget* item = (cocos2d::ui::Widget*)vec_actionItem[i]->getChildByName("item");
		cocos2d::ui::ImageView* icon = (cocos2d::ui::ImageView*)item->getChildByName("box")->getChildByName("icon");

		std::string iconstr = StringUtils::format("ui/%s.png", GlobalData::map_buidACData[name].at(i).icon);
		icon->loadTexture(iconstr, cocos2d::ui::TextureResType::PLIST);
		icon->setContentSize(Sprite::createWithSpriteFrameName(iconstr)->getContentSize());

		cocos2d::ui::Button* actbtn = (cocos2d::ui::Button*)item->getChildByName("actionbtn");
		actbtn->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onAction, this));
		actbtn->setTag(ACTION + i);
		actbtn->setTitleText(GlobalData::map_buidACData[name].at(i).actext);
		vec_actionbtn.push_back(actbtn);

		cocos2d::ui::LoadingBar* actloadbar = (cocos2d::ui::LoadingBar*)item->getChildByName("loadingbar");
		vec_actionbar.push_back(actloadbar);

		cocos2d::ui::Text* desc = (cocos2d::ui::Text*)item->getChildByName("desc");

		if (m_build->data.level >= GlobalData::map_buidACData[name].at(i).blv)
		{
			for (unsigned int m = 0; m < GlobalData::map_buidACData[name].at(i).res.size(); m++)
			{
				int restypecount = GlobalData::map_buidACData[name].at(i).res.at(m);
				if (restypecount > 0)
				{
					std::string str = StringUtils::format("res%d", m);
					cocos2d::ui::Widget* resitem = (cocos2d::ui::Widget*)item->getChildByName(str);
					resitem->setVisible(true);

					str = StringUtils::format("ui/%d.png", restypecount / 1000);
					Sprite* res = Sprite::createWithSpriteFrameName(str);
					res->setPosition(Vec2(resitem->getContentSize().width / 2, resitem->getContentSize().height / 2));
					res->setScale(0.38f);
					resitem->addChild(res);

					str = StringUtils::format("count%d", m);
					cocos2d::ui::Text* rescount = (cocos2d::ui::Text*)item->getChildByName(str);
					rescount->setVisible(true);

					std::string strid = StringUtils::format("%d", restypecount / 1000);
					int hascount = StorageRoom::getCountById(strid);
					int needcount = restypecount % 1000;
					str = StringUtils::format("%d/%d", hascount, needcount);
					rescount->setString(str);
					if (hascount < needcount)
						rescount->setTextColor(Color4B::RED);
				}
			}
			actbtn->setEnabled(true);
			desc->setVisible(false);
		}
		else
		{
			actbtn->setEnabled(false);
			
			desc->setVisible(true);
		}
	}
}

void BuildingUILayer::onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	int tag = node->getTag();
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (tag == BUILD)
		{
			for (unsigned int i = 0; i < m_build->data.Res[m_build->data.level].size(); i++)
			{
				int restypecount = m_build->data.Res[m_build->data.level].at(i);
				std::string strid = StringUtils::format("%d", restypecount / 1000);
				if (StorageRoom::getCountById(strid) < restypecount%1000)
				{
					HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("没有足够的资源!"));
					addChild(layer);
					return;
				}
			}

			buildbtn->setEnabled(false);
			buildbar->runAction(Sequence::create(MyProgressTo::create(ACTION_BAR_TIME, 100), CallFuncN::create(CC_CALLBACK_1(BuildingUILayer::onfinish, this, BUILD)), NULL));
			m_build->build();
		}
		else
		{
			for (unsigned int m = 0; m < GlobalData::map_buidACData[m_build->data.name].at(tag - ACTION).res.size(); m++)
			{
				int restypecount = GlobalData::map_buidACData[m_build->data.name].at(tag - ACTION).res.at(m);
				std::string strid = StringUtils::format("%d", restypecount / 1000);
				if (StorageRoom::getCountById(strid) < restypecount % 1000)
				{
					HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("没有足够的资源!"));
					this->addChild(layer);
					return;
				}
			}

			for (unsigned int i = 0; i < vec_actionbtn.size(); i++)
				vec_actionbtn[i]->setEnabled(false);

			int actime = GlobalData::map_buidACData[m_build->data.name].at(tag - ACTION).actime;
			int extime = GlobalData::map_buidACData[m_build->data.name].at(tag - ACTION).extime;

			if (actime < TIMESCALE * ACTION_BAR_TIME)
			{
				m_build->setActionBarTime(actime / TIMESCALE);
			}
			else
			{
				m_build->setActionBarTime(ACTION_BAR_TIME);
			}

			vec_actionbar[tag - ACTION]->runAction(Sequence::create(MyProgressTo::create(m_build->getActionBarTime(), 100), CallFuncN::create(CC_CALLBACK_1(BuildingUILayer::onfinish, this, (BACTIONTYPE)tag)), NULL));

			m_build->action(actime, extime);
		}
	}
}

void BuildingUILayer::onfinish(Ref* pSender, BACTIONTYPE type)
{
	g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
	if (type == BUILD)
	{
		for (unsigned int i = 0; i < m_build->data.Res[m_build->data.level - 1].size(); i++)
		{
			int restypecount = m_build->data.Res[m_build->data.level - 1].at(i);
			std::string strid = StringUtils::format("%d", restypecount / 1000);
			StorageRoom::use(strid, restypecount % 1000);
		}

		updataBuildRes();
		buildbar->setPercent(0);
		loadActionUi();
	}
	else
	{
		for (unsigned int i = 0; i < vec_actionbtn.size(); i++)
			vec_actionbtn[i]->setEnabled(true);
		vec_actionbar[type - ACTION]->setPercent(0);
		std::string strid = GlobalData::map_buidACData[m_build->data.name].at(type - ACTION).icon;
		if (strid.length() > 0 && strid.compare(0,1, "0") != 0)
		{
			PackageData data;
			data.type = GlobalData::map_buidACData[m_build->data.name].at(type - ACTION).type - 1;
			std::string idstr = StringUtils::format("%s", strid.c_str());
			data.strid = idstr;
			data.count = 1;
			data.lv = 0;
			data.exp = 0;
			data.goodvalue = 100;
			data.extype = GlobalData::map_buidACData[m_build->data.name].at(type - ACTION).extype;
			data.name = GlobalData::map_buidACData[m_build->data.name].at(type - ACTION).cname;
			data.desc = GlobalData::map_buidACData[m_build->data.name].at(type - ACTION).desc;
			StorageRoom::add(data);
		}

		for (unsigned int m = 0; m < GlobalData::map_buidACData[m_build->data.name].at(type - ACTION).res.size(); m++)
		{
			int restypecount = GlobalData::map_buidACData[m_build->data.name].at(type - ACTION).res.at(m);
			std::string strid = StringUtils::format("%d", restypecount / 1000);
			StorageRoom::use(strid, restypecount % 1000);
		}

		updataActionRes();
		updataBuildRes();
	}
}

void BuildingUILayer::updataBuildRes()
{
	cocos2d::ui::Widget* mainitem = (cocos2d::ui::Widget*)buildnode->getChildByName("item");

	int level = m_build->data.level;

	if (level > 0)
	{
		scrollview->setVisible(true);
		if (level == m_build->data.maxlevel)
		{
			buildbar->setVisible(false);
			buildnode->getChildByName("item")->getChildByName("progressbg")->setVisible(false);
			buildbtn->setEnabled(false);
			buildbtn->setTitleText(CommonFuncs::gbk2utf("最高级"));
		}
		else
		{
			buildbtn->setTitleText(CommonFuncs::gbk2utf("升级"));
		}
	}

	if (level >= m_build->data.maxlevel)
		level = m_build->data.maxlevel - 1;

	for (unsigned int i = 0; i < m_build->data.Res[level].size(); i++)
	{
		std::string str = StringUtils::format("res%d", i);
		cocos2d::ui::Widget* resitem = (cocos2d::ui::Widget*)mainitem->getChildByName(str);

		str = StringUtils::format("count%d", i);
		cocos2d::ui::Text* rescount = (cocos2d::ui::Text*)mainitem->getChildByName(str);

		if (m_build->data.level < m_build->data.maxlevel)
		{
			int restypecount = m_build->data.Res[level].at(i);
			if (restypecount > 0)
			{
				resitem->setVisible(true);

				str = StringUtils::format("ui/%d.png", restypecount / 1000);
				Sprite* res = Sprite::createWithSpriteFrameName(str);
				res->setPosition(Vec2(resitem->getContentSize().width / 2, resitem->getContentSize().height / 2));
				res->setScale(0.38f);
				resitem->addChild(res);
				std::string strid = StringUtils::format("%d", restypecount / 1000);
				int hascount = StorageRoom::getCountById(strid);
				int needcount = restypecount % 1000;
				str = StringUtils::format("%d/%d", hascount, needcount);
				rescount->setVisible(true);
				rescount->setString(str);
				if (hascount < needcount)
					rescount->setTextColor(Color4B::RED);
			}
			buildbtn->setEnabled(true);
		}
		else
		{
			resitem->setVisible(false);
			rescount->setVisible(false);
		}
	}
}

void BuildingUILayer::updataActionRes()
{
	std::string name = m_build->data.name;

	int size = GlobalData::map_buidACData[name].size();

	for (int i = 0; i < size; i++)
	{
		cocos2d::ui::Widget* item = (cocos2d::ui::Widget*)vec_actionItem[i]->getChildByName("item");
		if (m_build->data.level >= GlobalData::map_buidACData[name].at(i).blv)
		{
			for (unsigned int m = 0; m < GlobalData::map_buidACData[m_build->data.name].at(i).res.size(); m++)
			{
				int restypecount = GlobalData::map_buidACData[m_build->data.name].at(i).res.at(m);
				if (restypecount > 0)
				{
					std::string str = StringUtils::format("count%d", m);
					cocos2d::ui::Text* rescount = (cocos2d::ui::Text*)item->getChildByName(str);
					rescount->setVisible(true);

					std::string strid = StringUtils::format("%d", restypecount / 1000);
					int hascount = StorageRoom::getCountById(strid);
					int needcount = restypecount % 1000;
					str = StringUtils::format("%d/%d", hascount, needcount);
					rescount->setString(str);
					if (hascount < needcount)
						rescount->setTextColor(Color4B::RED);
				}
			}
		}
	}
}