#include "MixSuggestLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "SoundManager.h"
#include "WaitingProgress.h"
#include "GameDataSave.h"
#include "HintBox.h"
#include "MD5.h"

MixSuggestLayer::MixSuggestLayer()
{
	startime = 0;
}


MixSuggestLayer::~MixSuggestLayer()
{

}

bool MixSuggestLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	csbroot = CSLoader::createNode("mixSuggestLayer.csb");
	this->addChild(csbroot);

	for (int i = 0; i < 4; i++)
	{
		std::string str = StringUtils::format("mixbox_%d", i);

		img[i] = (cocos2d::ui::ImageView*)csbroot->getChildByName(str)->getChildByName("img");

		str = StringUtils::format("name%d", i);
		name[i] = (cocos2d::ui::Text*)csbroot->getChildByName(str);
	}

	freebtn = (cocos2d::ui::Button*)csbroot->getChildByName("freebtn");
	freebtn->addTouchEventListener(CC_CALLBACK_2(MixSuggestLayer::onFree, this));
	freebtn->setEnabled(false);

	silverbtn = (cocos2d::ui::Button*)csbroot->getChildByName("silverbtn");
	silverbtn->addTouchEventListener(CC_CALLBACK_2(MixSuggestLayer::onSilver, this));

	goldbtn = (cocos2d::ui::Button*)csbroot->getChildByName("goldbtn");
	goldbtn->addTouchEventListener(CC_CALLBACK_2(MixSuggestLayer::onGold, this));

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbroot->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(MixSuggestLayer::onBack, this));

	freetimelbl = (cocos2d::ui::Text*)csbroot->getChildByName("freetime");
	freetimelbl->setVisible(false);

	std::string suggestgfstr = GameDataSave::getInstance()->getSuggestMixGf();

	if (suggestgfstr.length() > 0)
		loadMixGfUi(GlobalData::map_MixGfData[suggestgfstr]);

	startime = GameDataSave::getInstance()->getFreeMixTime();

	getServerTime();

	auto m_listener = EventListenerTouchOneByOne::create();
	m_listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	m_listener->setSwallowTouches(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);

	return true;
}

void MixSuggestLayer::getServerTime()
{
	GlobalData::isGetServerData = true;
	WaitingProgress* waitbox = WaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::getInstance()->setDelegate(this);
	ServerDataSwap::getInstance()->getServerTime();
}

void MixSuggestLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void MixSuggestLayer::onExit()
{
	Layer::onExit();
}

void MixSuggestLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void MixSuggestLayer::onFree(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		freebtn->setEnabled(false);
		freetimelbl->setEnabled(true);
		startime = severtime;
		GameDataSave::getInstance()->setFreeMixTime(startime);
		randMixGf(0);
	}
}

void MixSuggestLayer::onSilver(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (StorageRoom::getCountById("80") < 50)
		{
			HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("银两不足！"));
			this->addChild(hbox);
		}
		else
		{
			StorageRoom::use("80", 50);
			randMixGf(1);
		}
	}
}

void MixSuggestLayer::onGold(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int mygold = GlobalData::getMyGoldCount();

		if (mygold > 50)
		{
			if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
			{
				GlobalData::dataIsModified = true;
				GlobalData::setMyGoldCount(0);
				HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
				this->addChild(hint);
				return;
			}

			GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - 50);
			randMixGf(2);
		}
		else
		{
			HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("金元宝不足！"));
			this->addChild(hbox);
		}
	}
}

void MixSuggestLayer::onSuccess()
{
	GlobalData::isGetServerData = false;
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	severtime = GlobalData::servertime;

	this->unschedule(schedule_selector(MixSuggestLayer::updateServerTime));
	this->schedule(schedule_selector(MixSuggestLayer::updateServerTime), 1.0f);

	if (severtime - startime >= 86400)
	{
		freebtn->setEnabled(true);
		freetimelbl->setVisible(false);
	}
}

void MixSuggestLayer::onErr(int errcode)
{
	GlobalData::isGetServerData = false;
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

}

void MixSuggestLayer::updateServerTime(float dt)
{
	severtime++;

	if (severtime - startime >= 86400)
	{
		freebtn->setEnabled(true);
		freetimelbl->setVisible(false);
	}
	else
	{
		freebtn->setEnabled(false);
		freetimelbl->setVisible(true);

		int lefttime = 86400 - (severtime - startime);
		std::string str = StringUtils::format("%02d:%02d:%02d", lefttime / 3600, lefttime % 3600 / 60, lefttime % 3600 % 60);
		freetimelbl->setString(str);
	}
}

void MixSuggestLayer::randMixGf(int type)
{
	int count = GameDataSave::getInstance()->getMixGfCountByType(type);

	std::map<int, std::vector<MixGfData>> map_bylvMixGf;
	std::map<std::string, MixGfData>::iterator it;

	for (it = GlobalData::map_MixGfData.begin(); it != GlobalData::map_MixGfData.end(); ++it)
	{
		int lv = GlobalData::map_MixGfData[it->first].lv;
		map_bylvMixGf[lv].push_back(GlobalData::map_MixGfData[it->first]);
	}
	int r[][4] = { { 60, 100, 100, 100 }, { 40, 70, 98, 100 }, { 0, 50, 95, 100 } };

	int lvrand = GlobalData::createRandomNum(100);

	int index = 0;
	for (int i = 0; i < 4; i++)
	{
		if (lvrand < r[type][i])
		{
			index = i;
			break;
		}
	}
	if (count > 0 && count % 10 == 0)
	{
		if (type == 0)
			index = 1;
		else if (type == 1)
			index = 2;
		else if (type == 2)
			index = 3;
	}

	int size = map_bylvMixGf[index + 1].size();
	int gfrand = GlobalData::createRandomNum(size);

	MixGfData mixdata = map_bylvMixGf[index + 1][gfrand];

	loadMixGfUi(mixdata);

	GameDataSave::getInstance()->setMixGfCountByType(type, count + 1);
	GameDataSave::getInstance()->setSuggestMixGf(mixdata.id);
}

void MixSuggestLayer::loadMixGfUi(MixGfData mixdata)
{
	std::string str = StringUtils::format("ui/%s.png", mixdata.mastergf.c_str());
	img[0]->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);
	name[0]->setString(GlobalData::map_allResource[mixdata.mastergf].cname);

	int secsize = mixdata.vec_secgf.size();
	for (int i = 0; i < 3; i++)
	{
		if (i < secsize)
		{
			str = StringUtils::format("ui/%s.png", mixdata.vec_secgf[i].c_str());
			name[i + 1]->setString(GlobalData::map_allResource[mixdata.vec_secgf[i]].cname);
		}
		else
		{
			str = "ui/mixsectext.png";
			name[i + 1]->setString("");
		}
		img[i + 1]->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);
	}
}
