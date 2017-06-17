#include "ActivitScene.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "Const.h"
#include "GlobalData.h"
#include "MyPackage.h"
#include "StorageRoom.h"

int ActivitScene::count = 0;
ActivitScene::ActivitScene()
{
	count++;
}


ActivitScene::~ActivitScene()
{
	--count;
	if (count > 0)
	{
		count = 0;
		Director::getInstance()->popScene();
	}
}

Scene* ActivitScene::createScene(std::string imagepath, std::string content)
{
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

	if (imagepath.compare("images/cday.jpg") == 0 && content.compare(CommonFuncs::gbk2utf("今夜很平静，新的一天开始...")) == 0)
	{
		Sprite* night = Sprite::create("images/cdaynight.png");
		night->setPosition(image->getPosition());
		this->addChild(night);
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
					allStorageData.push_back(sdata);
				}
			}
		}
		if (allStorageData.size() > 0)
		{
			int rcount = allStorageData.size() < 5 ? allStorageData.size() : 5;
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

				Label * namelbl = Label::createWithTTF(tmpdata.name, "fonts/STXINGKA.TTF", 25);
				namelbl->setColor(Color3B(255, 255, 255));
				namelbl->setPosition(Vec2(box->getPositionX(), 75));
				this->addChild(namelbl);

				int rndcount = tmpdata.count < 5 ? tmpdata.count : 5;
				int r1 = GlobalData::createRandomNum(rndcount) + 1;

				std::string strcount = StringUtils::format("x%d", r1);
				Label * coutlbl = Label::createWithSystemFont(strcount, "", 25);
				coutlbl->setAnchorPoint(Vec2(0, 0.5f));
				coutlbl->setColor(Color3B(255, 255, 255));
				coutlbl->setPosition(Vec2(box->getPositionX() + 40, 100));
				this->addChild(coutlbl);

				StorageRoom::use(tmpdata.strid, r1);
			}
		}
		else
		{
			text->setString(CommonFuncs::gbk2utf("盗贼到来，没有找到任何可偷的物品..."));
		}

	}

	this->scheduleOnce(schedule_selector(ActivitScene::popself), 2.0f);
	return true;
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

