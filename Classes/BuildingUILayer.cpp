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
#include "SoundManager.h"
#include "ResDetailsLayer.h"
#include "BuildingDetailsLayer.h"

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

	// ui
	m_csbnode = CSLoader::createNode("buidingUiLayer.csb");
	m_csbnode->setPosition(Vec2(0, -90));
	this->addChild(m_csbnode);

	//建筑物名称
	cocos2d::ui::Text* title = (cocos2d::ui::Text*)m_csbnode->getChildByName("title");
	title->setString(m_build->data.cname);

	//返回按钮
	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onBack, this));
	scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");
	if (m_build->data.level <= 0)
		scrollview->setVisible(false);

	//建筑物每个操作 node
	buildnode = CSLoader::createNode("actionNode.csb");
	buildnode->setPosition(Vec2(360, 860));
	m_csbnode->addChild(buildnode);

	//建造按钮
	buildbtn = (cocos2d::ui::Button*)buildnode->getChildByName("item")->getChildByName("actionbtn");
	buildbtn->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onAction, this));
	buildbtn->setTag(BUILD);

	//建筑物图标
	cocos2d::ui::ImageView* buildicon = (cocos2d::ui::ImageView*)buildnode->getChildByName("item")->getChildByName("box")->getChildByName("icon");
	std::string iconstr = StringUtils::format("ui/s%s.png", m_build->data.name);
	buildicon->loadTexture(iconstr, cocos2d::ui::TextureResType::PLIST);
	buildicon->setContentSize(Sprite::createWithSpriteFrameName(iconstr)->getContentSize());
	buildicon->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onBuidingDetails, this));


	//建筑升级进度条
	buildbar = (cocos2d::ui::LoadingBar*)buildnode->getChildByName("item")->getChildByName("loadingbar");

	updataBuildRes();

	loadActionUi();

	////layer 点击事件，屏蔽下层事件
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
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
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
			acnode->setPosition(Vec2(scrollview->getContentSize().width / 2 - 13, scrollinnerheight - itemheight / 2 - i * itemheight));

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
					resitem->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onResDetails, this));
					resitem->setTag((i +1)* 100 + m);
					resitem->setVisible(true);

					str = StringUtils::format("ui/%d.png", restypecount / 1000);//资源图标
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
					str = StringUtils::format("%d/%d", hascount, needcount);//拥有的资源个数/需要资源个数
					rescount->setString(str);
					if (hascount < needcount)
						rescount->setTextColor(Color4B::RED);
					else
						rescount->setTextColor(Color4B::BLACK);
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
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		if (tag == BUILD)//建造或者升级
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
		else//操作
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
	if (type == BUILD)//建造或升级完成
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
		std::string text = "建造成功!";
		if (m_build->data.level > 1)
			text = "升级完成";
		HintBox* layer = HintBox::create(CommonFuncs::gbk2utf(text.c_str()));
		this->addChild(layer);
		
	}
	else//操作完成
	{
		for (unsigned int i = 0; i < vec_actionbtn.size(); i++)
			vec_actionbtn[i]->setEnabled(true);
		vec_actionbar[type - ACTION]->setPercent(0);
		std::string strid = GlobalData::map_buidACData[m_build->data.name].at(type - ACTION).icon;
		//是否是产出新的物品，（睡觉和暖炉不会产出新的物品 icon以“0-”开头，其他建筑物的操作或产出新的物品，eg:制作烤肉）
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

		HintBox* layer = HintBox::create(CommonFuncs::gbk2utf("制作成功"));
		this->addChild(layer);
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

	int ressize = m_build->data.Res[level].size();

	if (ressize > 0)
	{
		//更新升级需要的资源
		for (unsigned int i = 0; i < m_build->data.Res[level].size(); i++)
		{
			std::string str = StringUtils::format("res%d", i);
			cocos2d::ui::Widget* resitem = (cocos2d::ui::Widget*)mainitem->getChildByName(str);

			resitem->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onResDetails, this));
			resitem->setTag(i);

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
					else
						rescount->setTextColor(Color4B::BLACK);
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
	else
	{
		cocos2d::ui::Text* desc = (cocos2d::ui::Text*)mainitem->getChildByName("desc");
		desc->setString(m_build->data.cname);
	}
}

void BuildingUILayer::updataActionRes()
{
	std::string name = m_build->data.name;

	int size = GlobalData::map_buidACData[name].size();

	//更新操作需要的资源
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
					else
						rescount->setTextColor(Color4B::BLACK);
				}
			}
		}
	}
}

void BuildingUILayer::onResDetails(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int tag = node->getTag();
		int intresid = 0;
		if (tag >= 100)//需要合成的
		{
			intresid = GlobalData::map_buidACData[m_build->data.name][tag / 100 - 1].res[tag % 100];
	
		}
		else//建筑物的
		{
			intresid = m_build->data.Res[m_build->data.level][tag];
		}
		std::string strid = StringUtils::format("%d", intresid / 1000);
		ResDetailsLayer::whereClick = 1;
		this->addChild(ResDetailsLayer::createByResId(strid));
	}
}

void BuildingUILayer::onBuidingDetails(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->addChild(BuildingDetailsLayer::create(m_build));
	}
}