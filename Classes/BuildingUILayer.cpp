#include "BuildingUILayer.h"
#include "json.h"
#include "Bed.h"
#include "MyActionProgressTimer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "Const.h"

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
	m_csbnode->setPosition(Vec2(0, -100));
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

	updataBuildRes();

	parseBuildActionJSon();
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

	int size = map_buidACData[name].size();

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
		
		std::string iconstr = StringUtils::format("ui/%s.png", map_buidACData[name].at(i).icon);
		icon->loadTexture(iconstr, cocos2d::ui::TextureResType::PLIST);
		icon->setContentSize(Sprite::createWithSpriteFrameName(iconstr)->getContentSize());

		cocos2d::ui::Button* actbtn = (cocos2d::ui::Button*)item->getChildByName("actionbtn");
		actbtn->addTouchEventListener(CC_CALLBACK_2(BuildingUILayer::onAction, this));
		actbtn->setTag(ACTION + i);
		actbtn->setTitleText(map_buidACData[name].at(i).actext);
		vec_actionbtn.push_back(actbtn);

		cocos2d::ui::LoadingBar* actloadbar = (cocos2d::ui::LoadingBar*)item->getChildByName("loadingbar");
		vec_actionbar.push_back(actloadbar);

		cocos2d::ui::Text* desc = (cocos2d::ui::Text*)item->getChildByName("desc");

		if (m_build->data.level >= map_buidACData[name].at(i).blv)
		{
			for (unsigned int m = 0; m < map_buidACData[name].at(i).res.size(); m++)
			{
				int restypecount = map_buidACData[name].at(i).res.at(m);
				if (restypecount > 0)
				{
					std::string str = StringUtils::format("res%d", m);
					cocos2d::ui::Widget* resitem = (cocos2d::ui::Widget*)item->getChildByName(str);
					resitem->setVisible(true);

					str = StringUtils::format("ui/res%d.png", restypecount / 1000);
					Sprite* res = Sprite::createWithSpriteFrameName(str);
					res->setPosition(Vec2(resitem->getContentSize().width / 2, resitem->getContentSize().height / 2));
					resitem->addChild(res);

					str = StringUtils::format("count%d", m);
					cocos2d::ui::Text* rescount = (cocos2d::ui::Text*)item->getChildByName(str);
					rescount->setVisible(true);

					str = StringUtils::format("%d/%d", restypecount % 1000, restypecount % 1000);
					rescount->setString(str);
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

void BuildingUILayer::parseBuildActionJSon()
{
	std::string jsonfilename = StringUtils::format("data/%s.json", m_build->data.name);
	rapidjson::Document doc = ReadJsonFile(jsonfilename);
	rapidjson::Value& bc = doc["bc"];
	for (unsigned int i = 0; i < bc.Size(); i++)
	{
		BuildActionData data;
		rapidjson::Value& jsonvalue = bc[i];
		if (jsonvalue.IsObject())
		{
			rapidjson::Value& value = jsonvalue["icon"];
			strcpy(data.icon, value.GetString());

			value = jsonvalue["blv"];
			data.blv = value.GetInt();

			value = jsonvalue["time"];
			data.actime = value.GetInt();

			if (jsonvalue.HasMember("extime"))
			{
				value = jsonvalue["extime"];
				data.extime = value.GetInt();
			}
			else
				data.extime = 0;

			value = jsonvalue["actext"];
			strcpy(data.actext, value.GetString());

			value = jsonvalue["res"];
			for (unsigned int i = 0; i < value.Size(); i++)
			{
				data.res.push_back(value[i].GetInt());
			}
			map_buidACData[m_build->data.name].push_back(data);
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
			buildbtn->setEnabled(false);
			buildbar = (cocos2d::ui::LoadingBar*)buildnode->getChildByName("item")->getChildByName("loadingbar");
			buildbar->runAction(Sequence::create(MyProgressTo::create(ACTION_BAR_TIME, 100), CallFuncN::create(CC_CALLBACK_1(BuildingUILayer::onfinish, this, BUILD)), NULL));
			m_build->build();
		}
		else
		{
			for (unsigned int i = 0; i < vec_actionbtn.size(); i++)
				vec_actionbtn[i]->setEnabled(false);
			vec_actionbar[tag - ACTION]->runAction(Sequence::create(MyProgressTo::create(ACTION_BAR_TIME, 100), CallFuncN::create(CC_CALLBACK_1(BuildingUILayer::onfinish, this, (BACTIONTYPE)tag)), NULL));
			int actime = map_buidACData[m_build->data.name].at(tag - ACTION).actime;
			int extime = map_buidACData[m_build->data.name].at(tag - ACTION).extime;
			m_build->action(actime, extime);
		}
	}
}

void BuildingUILayer::onfinish(Ref* pSender, BACTIONTYPE type)
{
	g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
	if (type == BUILD)
	{
		m_build->data.level++;
		if (m_build->data.level > 0)
		{
			scrollview->setVisible(true);
			buildbtn->setTitleText(CommonFuncs::gbk2utf("升级"));
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
	}
}

void BuildingUILayer::updataBuildRes()
{
	cocos2d::ui::Widget* mainitem = (cocos2d::ui::Widget*)buildnode->getChildByName("item");
	for (unsigned int i = 0; i < m_build->data.Res[m_build->data.level].size(); i++)
	{
		cocos2d::ui::Widget* resitem = NULL;
		cocos2d::ui::Text* rescount = NULL;
		if (m_build->data.level < m_build->data.maxlevel)
		{
			int restypecount = m_build->data.Res[m_build->data.level].at(i);
			if (restypecount > 0)
			{
				std::string str = StringUtils::format("res%d", i);
				resitem = (cocos2d::ui::Widget*)mainitem->getChildByName(str);
				resitem->setVisible(true);
				str = StringUtils::format("count%d", i);
				rescount = (cocos2d::ui::Text*)mainitem->getChildByName(str);

				str = StringUtils::format("ui/res%d.png", restypecount / 1000);
				Sprite* res = Sprite::createWithSpriteFrameName(str);
				res->setPosition(Vec2(resitem->getContentSize().width / 2, resitem->getContentSize().height / 2));
				resitem->addChild(res);

				str = StringUtils::format("%d/%d", restypecount % 1000, restypecount % 1000);
				rescount->setVisible(true);
				rescount->setString(str);
			}
			buildbtn->setEnabled(true);
		}
		else
		{
			resitem->setVisible(false);
			rescount->setVisible(false);
			buildbar->setVisible(false);
			buildnode->getChildByName("item")->getChildByName("progressbg")->setVisible(false);
			buildbtn->setTitleText(CommonFuncs::gbk2utf("最高级"));
		}
	}
}