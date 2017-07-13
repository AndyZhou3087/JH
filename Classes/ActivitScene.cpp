#include "ActivitScene.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "Const.h"
#include "GlobalData.h"
#include "MyPackage.h"
#include "StorageRoom.h"
#include "GameScene.h"

ActivitScene::ActivitScene()
{

}


ActivitScene::~ActivitScene()
{
	GlobalData::isPopingScene = false;
}

Scene* ActivitScene::createScene(std::string imagepath, std::string content)
{
	if (GlobalData::isPopingScene)
		return NULL;
	GlobalData::isPopingScene = true;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ActivitScene::create(imagepath, content);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool ActivitScene::init(std::string imagepath, std::string content)
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 255));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("activityLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::ImageView* image = (cocos2d::ui::ImageView*)csbnode->getChildByName("image");
	image->loadTexture(imagepath, cocos2d::ui::TextureResType::LOCAL);
	cocos2d::ui::Text* text = (cocos2d::ui::Text*)csbnode->getChildByName("text");
	text->setString(content);

	cocos2d::ui::Text* tips = (cocos2d::ui::Text*)csbnode->getChildByName("tips");
	int r = GlobalData::createRandomNum(sizeof(tipswords) / sizeof(tipswords[0]));
	std::string str = StringUtils::format("tips：%s", tipswords[r].c_str());
	tips->setString(CommonFuncs::gbk2utf(str.c_str()));

	float distime = 2.0f;
	if (imagepath.compare("images/cday.jpg") == 0 && content.compare(CommonFuncs::gbk2utf("今夜很平静，新的一天开始...")) == 0)
	{
		Sprite* night = Sprite::create("images/cdaynight.png");
		night->setPosition(image->getPosition());
		this->addChild(night);
	}
	else if (imagepath.compare("images/insect.jpg") == 0)
	{
		tips->setString(CommonFuncs::gbk2utf("后山的三七、降真香、野菜、水果数量减半！"));
		distime = 3.0f;
	}
	else if (imagepath.compare("images/hunter.jpg") == 0)
	{
		tips->setString(CommonFuncs::gbk2utf("最近上山打猎的人增多，兔子和狼都快被打没了！"));
		distime = 3.0f;
	}
	else if (imagepath.compare("images/thieves.jpg") == 0)
	{
		tips->setVisible(false);
		std::vector<PackageData> allStorageData;
		std::map<int, std::vector<PackageData>>::iterator it;
		for (it = StorageRoom::map_storageData.begin(); it != StorageRoom::map_storageData.end(); ++it)
		{
			int size = StorageRoom::map_storageData[it->first].size();
			for (int j = 0; j < size; j++)
			{
				PackageData sdata = StorageRoom::map_storageData[it->first][j];

				if (sdata.strid.compare("x001") != 0 && sdata.strid.compare("w001") != 0 && sdata.count > 0)
				{
					bool isadd = true;
					if ((sdata.type == WEAPON || sdata.type == PROTECT_EQU) && GlobalData::map_equips[sdata.strid].qu >= 5)
					{
						isadd = false;
					}
					else if ((sdata.type == N_GONG || sdata.type == W_GONG) && GlobalData::map_wgngs[sdata.strid].qu >= 5)
					{
						isadd = false;
					}
					else if (sdata.type == RES_2 && atoi(sdata.strid.c_str()) >= 70 && atoi(sdata.strid.c_str()) <= 78)
					{
						isadd = false;
					}
					if (isadd)
						allStorageData.push_back(sdata);
				}
			}
		}
		if (allStorageData.size() > 0)
		{
			int rcount = allStorageData.size() < 3 ? allStorageData.size() : 3;
			int r = GlobalData::createRandomNum(rcount) + 1;
			int startx[] = { 360, 300, 240, 180, 120 };
			int spacex[] = { 120, 120, 120, 120, 120 };
			srand(GlobalData::getSysSecTime());
			std::random_shuffle(allStorageData.begin(), allStorageData.end());
			for (int i = 0; i < r; i++)
			{
				std::string boxstr = "ui/buildsmall.png";
				PackageData tmpdata = allStorageData[i];
				if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
				{
					boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata.strid].qu);
				}
				else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
				{
					boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata.strid].qu);
				}

				Sprite * box = Sprite::createWithSpriteFrameName(boxstr);
				box->setPosition(Vec2(startx[r - 1] + i*spacex[r - 1], 130));
				this->addChild(box);

				std::string str = StringUtils::format("ui/%s.png", tmpdata.strid.c_str());
				Sprite* res = Sprite::createWithSpriteFrameName(str);
				res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2));
				box->addChild(res);
				box->setScale(0.6f);

				Label * namelbl = Label::createWithTTF(GlobalData::map_allResource[tmpdata.strid].cname, "fonts/STXINGKA.TTF", 25);
				namelbl->setColor(Color3B(255, 255, 255));
				namelbl->setPosition(Vec2(box->getPositionX(), 75));
				this->addChild(namelbl);

				int rndcount = tmpdata.count < 3 ? tmpdata.count : 3;
				int r1 = GlobalData::createRandomNum(rndcount) + 1;

				std::string strcount = StringUtils::format("x%d", r1);
				Label * coutlbl = Label::createWithSystemFont(strcount, "", 25);
				coutlbl->setAnchorPoint(Vec2(0, 0.5f));
				coutlbl->setColor(Color3B(255, 255, 255));
				coutlbl->setPosition(Vec2(box->getPositionX() + 40, 100));
				this->addChild(coutlbl);

				StorageRoom::use(tmpdata.strid, r1);
			}
			distime = 3.0f;
		}
		else
		{
			text->setString(CommonFuncs::gbk2utf("盗贼到来，没有找到任何可偷的物品..."));
		}

	}
	else if (imagepath.compare("images/findtreasure.jpg") == 0)
	{
		int rj = GlobalData::createRandomNum(100);
		if (rj < 30)
		{
			image->loadTexture("images/jumpnoting.jpg", cocos2d::ui::TextureResType::LOCAL);
			text->setString(CommonFuncs::gbk2utf("跳崖摔伤"));
			float val = g_hero->getInnerinjuryValue();
			if (val < 30)
				g_hero->setInnerinjuryValue(0);
			else
				g_hero->setInnerinjuryValue(val - 30);

			val = g_hero->getOutinjuryValue();

			if (val < 30)
				g_hero->setOutinjuryValue(0);
			else
				g_hero->setOutinjuryValue(val - 30);

			val = g_hero->getSpiritValue();

			if (val < 30)
				g_hero->setSpiritValue(0);
			else
				g_hero->setSpiritValue(val - 30);

			val = g_hero->getHungerValue();
			if (val < 30)
				g_hero->setHungerValue(0);
			else
				g_hero->setHungerValue(val - 30);

			g_hero->setLifeValue(g_hero->getLifeValue() * 0.7f);
			tips->setString(CommonFuncs::gbk2utf("没想到这个悬崖这么高！！！受伤严重！！！"));
			distime = 4.0f;
		}
		else
		{
			int rf = GlobalData::createRandomNum(100);
			if (rf < 20)
			{
				image->loadTexture("images/jumpnoting.jpg", cocos2d::ui::TextureResType::LOCAL);
				text->setString(CommonFuncs::gbk2utf("一无所获"));
				tips->setString(CommonFuncs::gbk2utf("这次白跳了，什么都没有，发光的只不过时一个破石头在反光。"));
				distime = 3.0f;
			}
			else
			{
				this->scheduleOnce(schedule_selector(ActivitScene::getRndRes), 0.2f);
				//text->setString(CommonFuncs::gbk2utf("机缘巧合"));
				tips->setString(CommonFuncs::gbk2utf("果然没有白白冒险，没想到这悬崖下别有洞天！你获得了以下物品："));
				tips->setPositionY(text->getPositionY() + 20);
				return true;
			}
		}
	}

	this->scheduleOnce(schedule_selector(ActivitScene::popself), distime);
	return true;
}

void ActivitScene::getRndRes(float dt)
{
	std::vector<std::string> vec_tempgf;
	std::vector<std::string> vec_tempequip;
	std::vector<PackageData> vec_randData;
	std::map<std::string, WG_NGData>::iterator it;
	for (it = GlobalData::map_wgngs.begin(); it != GlobalData::map_wgngs.end(); ++it)
	{
		WG_NGData gfdata = GlobalData::map_wgngs[it->first];
		if (!g_hero->checkifHasGF_Equip(gfdata.id) && gfdata.qu >= 4)
			vec_tempgf.push_back(gfdata.id);
	}

	std::map<std::string, EquipData>::iterator ite;
	for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
	{
		EquipData edata = GlobalData::map_equips[ite->first];
		if (!g_hero->checkifHasGF_Equip(edata.id) && edata.qu >= 4)
			vec_tempequip.push_back(edata.id);
	}
	std::vector<std::string> vec_randRes;
	int r = GlobalData::createRandomNum(100);
	if (r < 40)
	{
		for (unsigned int i = 0; i < vec_tempgf.size(); i++)
		{
			if (!GlobalData::tempHasGf_Equip(vec_tempgf[i]))
			{
				vec_randRes.push_back(vec_tempgf[i]);
			}
		}
		if (vec_randRes.size() > 0)
		{
			r = GlobalData::createRandomNum(vec_randRes.size());
			PackageData data;
			data.strid = vec_randRes[r];
			data.type = GlobalData::map_wgngs[data.strid].type - 1;
			data.count = 1;
			vec_randData.push_back(data);
		}
	}

	vec_randRes.clear();

	int r1 = GlobalData::createRandomNum(100);
	if (r1 < 40)
	{
		for (unsigned int i = 0; i < vec_tempequip.size(); i++)
		{
			if (!GlobalData::tempHasGf_Equip(vec_tempequip[i]))
				vec_randRes.push_back(vec_tempequip[i]);
		}
		if (vec_randRes.size() > 0)
		{
			r1 = GlobalData::createRandomNum(vec_randRes.size());

			PackageData data;
			data.strid = vec_randRes[r];
			data.type = GlobalData::map_wgngs[data.strid].type - 1;
			data.count = 1;
			vec_randData.push_back(data);
		}
	}

	vec_randRes.clear();

	std::vector<std::string> vec_resid;
	for (int i = 1; i <= 23; i++)
	{
		std::string strid = StringUtils::format("%d", i);
		vec_randRes.push_back(strid);
	}
	vec_randRes.push_back("80");

	srand(GlobalData::getSysSecTime());
	std::random_shuffle(vec_randRes.begin(), vec_randRes.end());
	int r3 = GlobalData::createRandomNum(3) + 1;
	for (int i = 0; i < r3; i++)
	{
		PackageData data;
		data.strid = vec_randRes[i];
		data.type = GlobalData::getResType(data.strid);
		data.count = 10;
		vec_randData.push_back(data);
	}

	int startx[] = { 360, 300, 240, 180, 120 };
	int spacex[] = { 120, 120, 120, 120, 120 };

	int datasize = vec_randData.size();
	for (int i = 0; i < datasize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = vec_randData[i];
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata.strid].qu);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata.strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);
		box->setPosition(Vec2(startx[datasize - 1] + i*spacex[datasize - 1], 130));
		this->addChild(box);

		std::string str = StringUtils::format("ui/%s.png", tmpdata.strid.c_str());
		Sprite* res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2));
		box->addChild(res);
		box->setScale(0.6f);

		Label * namelbl = Label::createWithTTF(GlobalData::map_allResource[tmpdata.strid].cname, "fonts/STXINGKA.TTF", 25);
		namelbl->setColor(Color3B(255, 255, 255));
		namelbl->setPosition(Vec2(box->getPositionX(), 75));
		this->addChild(namelbl);

		std::string strcount = StringUtils::format("x%d", tmpdata.count);
		Label * coutlbl = Label::createWithSystemFont(strcount, "", 25);
		coutlbl->setAnchorPoint(Vec2(0, 0.5f));
		coutlbl->setColor(Color3B(255, 255, 255));
		coutlbl->setPosition(Vec2(box->getPositionX() + 40, 100));
		this->addChild(coutlbl);

		StorageRoom::add(tmpdata);
	}

	this->scheduleOnce(schedule_selector(ActivitScene::popself), 4.0f);
}

ActivitScene* ActivitScene::create(std::string imagepath, std::string content)
{
	ActivitScene *pRet = new ActivitScene();
	if (pRet && pRet->init(imagepath, content))
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

void ActivitScene::popself(float dt)
{
	Director::getInstance()->popScene();
}