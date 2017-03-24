#include "TempStorageLayer.h"
#include "json.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "MyPackage.h"
#include "Const.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"

TempStorageLayer::TempStorageLayer()
{
}


TempStorageLayer::~TempStorageLayer()
{
}

TempStorageLayer* TempStorageLayer::create(std::string addrname)
{
	TempStorageLayer *pRet = new TempStorageLayer();
	if (pRet && pRet->init(addrname))
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

bool TempStorageLayer::init(std::string addrname)
{
	Node* csbnode = CSLoader::createNode("tempStorageLayer.csb");
	this->addChild(csbnode);
	m_addrname = addrname;

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(TempStorageLayer::onBack, this));

	cocos2d::ui::Button* getallbtn = (cocos2d::ui::Button*)csbnode->getChildByName("allgetbtn");
	getallbtn->addTouchEventListener(CC_CALLBACK_2(TempStorageLayer::onAllGet, this));

	m_scrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");

	loadTempData();

	updata();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void TempStorageLayer::onRewardItem(cocos2d::Ref* pSender)
{
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();

	removeitem();

	int count = data->count - 1;
	if (count <= 0)
	{
		std::vector<PackageData>::iterator it;
		for (it = tempResData.begin(); it != tempResData.end(); ++it)
		{
			if (it->id == data->id)
			{
				PackageData pdata;
				pdata.type = data->type;
				pdata.id = data->id;
				pdata.count = 1;
				if (MyPackage::add(pdata) == 0)
				{
					data->count--;
					tempResData.erase(it);
				}
				break;
			}
		}
	}
	else
	{
		PackageData pdata;
		pdata.type = data->type;
		pdata.id = data->id;
		pdata.count = 1;
		if (MyPackage::add(pdata) == 0)
		{
			data->count--;
		}
	}
	saveTempData();
	updata();
}

void TempStorageLayer::onPackageItem(cocos2d::Ref* pSender)
{
	removeitem();
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = MyPackage::vec_packages[index];
	unsigned int i = 0;
	for (i = 0; i < tempResData.size(); i++)
	{
		if (data.id == tempResData[i].id)
		{
			tempResData[i].count++;
			break;
		}
	}

	if (i == tempResData.size())
	{
		tempResData.push_back(data);
	}
	saveTempData();
	MyPackage::cutone(index);
	updata();
}


void TempStorageLayer::removeitem()
{
	for (unsigned int i = 0; i < tempResData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		m_scrollView->removeChildByName(name);
	}

	for (unsigned int i = 0; i < MyPackage::getSize(); i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		this->removeChildByName(name);
	}
}

void TempStorageLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}


void TempStorageLayer::loadTempData()
{
	std::string datastr = GameDataSave::getInstance()->getTempStorage(m_addrname);
	std::vector<std::string> vec_retstr;
	CommonFuncs::split(datastr, vec_retstr, ";");
	for (unsigned int i = 0; i < vec_retstr.size(); i++)
	{
		std::vector<std::string> tmp;
		CommonFuncs::split(vec_retstr[i], tmp, "-");
		int idtype = atoi(tmp[0].c_str());
		PackageData data;
		data.id = idtype/1000;
		data.type = idtype % 1000;
		data.count = atoi(tmp[1].c_str());
		tempResData.push_back(data);
	}
}

void TempStorageLayer::saveTempData()
{
	std::string str;
	for (unsigned int i = 0; i < tempResData.size(); i++)
	{
		std::string onestr = StringUtils::format("%d-%d;", tempResData[i].id * 1000 + tempResData[i].type, tempResData[i].count);
		str.append(onestr);
	}
	GameDataSave::getInstance()->setTempStorage(m_addrname, str.substr(0, str.length() - 1));
}

void TempStorageLayer::onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeitem();

		for (unsigned int i = 0; i < tempResData.size(); i++)
		{
			PackageData data = tempResData[i];
			data.count = 1;
			if (MyPackage::add(data) == 0)
			{
				if (--tempResData[i].count <= 0)
				{
					tempResData.erase(tempResData.begin() + i);
					break;
				}
			}
		}

		updata();
	}
}

void TempStorageLayer::updata()
{
	int tempsize = tempResData.size();
	int itemheight = 135;
	int row = tempsize % 5 == 0 ? tempsize / 5 : (tempsize / 5 + 1);
	int innerheight = itemheight * row;
	int contentheight = m_scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_scrollView->setInnerContainerSize(Size(650, innerheight));

	for (int i = 0; i < tempsize; i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(TempStorageLayer::onRewardItem, this));
		boxItem->setTag(i);
		boxItem->setUserData(&tempResData[i]);
		boxItem->setPosition(Vec2(70 + i % 5 * 135, innerheight - i/5 * itemheight - itemheight / 2));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		m_scrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%d.png", tempResData[i].id);
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", tempResData[i].count);
		Label * reslbl = Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}

	for (unsigned int i = 0; i < MyPackage::getSize(); i++)
	{

		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(TempStorageLayer::onPackageItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(110 + i * 125, 225));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%d.png", MyPackage::vec_packages[i].id);
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", MyPackage::vec_packages[i].count);
		Label * reslbl = Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}