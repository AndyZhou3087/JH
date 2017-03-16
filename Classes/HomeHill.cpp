#include "HomeHill.h"
#include "json.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "ActionGetLayer.h"
#include "Const.h"


HomeHill::HomeHill()
{
	pasttime = 0;
}


HomeHill::~HomeHill()
{
}

bool HomeHill::init()
{
	Node* csbnode = CSLoader::createNode("homeHillLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(HomeHill::onBack, this));

	loadJsonData();
	
	int ressize = vec_resid.size();
	scrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");

	int itemheight = 140;
	int innerheight = itemheight * ressize;
	int contentheight = scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollView->setInnerContainerSize(Size(650, innerheight));

	for (int i = 0; i < ressize; i++)
	{
		Node* resnode = CSLoader::createNode("resNode.csb");
		std::string namestr = StringUtils::format("node%d", i);
		scrollView->addChild(resnode,0, namestr);
		resnode->setPosition(Vec2(scrollView->getContentSize().width/2, innerheight - i * itemheight - itemheight / 2));

		cocos2d::ui::ImageView* iconimg = (cocos2d::ui::ImageView*)resnode->getChildByName("icon");
		cocos2d::ui::Text* name = (cocos2d::ui::Text*)resnode->getChildByName("name");
		cocos2d::ui::Text* count = (cocos2d::ui::Text*)resnode->getChildByName("count");
		cocos2d::ui::Text* speed = (cocos2d::ui::Text*)resnode->getChildByName("speed");
		cocos2d::ui::Text* waittime = (cocos2d::ui::Text*)resnode->getChildByName("waittime");
		cocos2d::ui::Text* desc = (cocos2d::ui::Text*)resnode->getChildByName("desc");
		cocos2d::ui::Text* waittext = (cocos2d::ui::Text*)resnode->getChildByName("waittext");
		
		cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)resnode->getChildByName("actionbtn");
		actionbtn->addTouchEventListener(CC_CALLBACK_2(HomeHill::onclick, this));
		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData data = GlobalData::vec_resData[m];
			if (vec_resid[i] == data.id)
			{
				actionbtn->setTag(m);
				std::string str = StringUtils::format("ui/%d.png", data.id);
				iconimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
				iconimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

				name->setString(data.cname);
				desc->setString(data.desc);
				str = StringUtils::format("%d", data.count);
				count->setString(str);

				float fs = ((float)data.speed) / 60.0f;
				str = StringUtils::format("%.1fh", fs);
				speed->setString(str);

				float wfs = fs * data.max;
				str = StringUtils::format("%.1fh", wfs);
				waittime->setString(str);

				if (data.count <= 0)
				{
					waittext->setVisible(true);
					waittime->setVisible(true);
					actionbtn->setEnabled(false);
					actionbtn->setBright(false);
				}
				else
				{
					waittext->setVisible(false);
					waittime->setVisible(false);
					actionbtn->setEnabled(true);
					actionbtn->setBright(true);
				}

				actionbtn->setTitleText(CommonFuncs::gbk2utf(acname[data.actype].c_str()));
				
			}
		}

	}
	this->schedule(schedule_selector(HomeHill::updateUI),0.2f);


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
void HomeHill::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void HomeHill::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;

		ResData* data = &GlobalData::vec_resData[node->getTag()];
		bool isself = true;
		for (unsigned int i = 0; i<data->res.size(); i++)
		{
			if (data->id != data->res.at(i))
			{
				isself = false;
			}
		}
		if (GlobalData::vec_resData[node->getTag()].count > 0)
		{
			GlobalData::vec_resData[node->getTag()].count--;
			ActionGetLayer* layer = ActionGetLayer::create(data->res, data->type, data->actype);
			this->addChild(layer);

		}
	}
}

void HomeHill::loadJsonData()
{
	rapidjson::Document doc = ReadJsonFile("data/homehill.json");
	rapidjson::Value& values = doc["sh"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		vec_resid.push_back(values[i].GetInt());
	}
}

void HomeHill::updateUI(float dt)
{
	pasttime += 1;
	for (unsigned int i = 0; i < vec_resid.size(); i++)
	{
		std::string namestr = StringUtils::format("node%d", i);
		Node* resnode = scrollView->getChildByName(namestr);

		cocos2d::ui::Text* count = (cocos2d::ui::Text*)resnode->getChildByName("count");
		cocos2d::ui::Text* speed = (cocos2d::ui::Text*)resnode->getChildByName("speed");
		cocos2d::ui::Text* waittime = (cocos2d::ui::Text*)resnode->getChildByName("waittime");
		cocos2d::ui::Text* waittext = (cocos2d::ui::Text*)resnode->getChildByName("waittext");
		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData* data = &GlobalData::vec_resData[m];
			if (vec_resid[i] == data->id)
			{
				std::string str = StringUtils::format("%d", data->count);
				count->setString(str);
				float fs = ((float)data->speed) / 60.0f;
				str = StringUtils::format("%.1fh", fs);
				speed->setString(str);

				float wfs = fs * data->max;
				str = StringUtils::format("%.1fh", wfs);
				waittime->setString(str);
				data->pastmin += pasttime;
				if (data->pastmin >= data->speed)
				{
					data->pastmin = 0;
					data->count++;
				}

				if (data->count <= 0)
				{
					waittext->setVisible(true);
					waittime->setVisible(true);
				}
				else
				{
					if (data->count >= data->max)
						data->count = data->max;
					waittext->setVisible(false);
					waittime->setVisible(false);
				}
			}
		}
	}
}