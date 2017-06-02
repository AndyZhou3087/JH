#include "ReviveLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "DeathLayer.h"
#include "SoundManager.h"
#include "ShopLayer.h"

bool ReviveLayer::isBuyRevive = false;
ReviveLayer::ReviveLayer()
{
	isBuyRevive = false;
}


ReviveLayer::~ReviveLayer()
{
	isBuyRevive = false;
}

bool ReviveLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("reviveLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cacelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onCancel, this));

	cocos2d::ui::Button* revivebtn = (cocos2d::ui::Button*)csbnode->getChildByName("revivebtn");
	revivebtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onRevive, this));

	revivecount = StorageRoom::getCountById("73");

	for (unsigned int i = 0; i < MyPackage::vec_packages.size(); i++)
	{
		if (MyPackage::vec_packages[i].strid.compare("73") == 0)
		{
			revivecount += MyPackage::vec_packages[i].count;
			break;
		}
	}

	std::string strcount = StringUtils::format("x%d", revivecount);

	cocos2d::ui::Text* revivecountlbl = (cocos2d::ui::Text*)csbnode->getChildByName("revivecount");
	revivecountlbl->setString(strcount);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

ReviveLayer* ReviveLayer::create()
{
	ReviveLayer *pRet = new ReviveLayer();
	if (pRet && pRet->init())
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

void ReviveLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		this->removeFromParentAndCleanup(true);
		if (g_gameLayer != NULL)
			g_gameLayer->removeChildByName("fightlayer");
		Director::getInstance()->resume();
		DeathLayer* layer = DeathLayer::create();
		Director::getInstance()->getRunningScene()->addChild(layer);
	}
}
void ReviveLayer::onRevive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

		if (revivecount > 0)
		{
			doRevive();
		}
		else
		{
			isBuyRevive = true;
			ShopLayer::beginPay(7);
		}
	}
}

void ReviveLayer::doRevive()
{
	if (g_gameLayer != NULL)
	{
		g_gameLayer->removeChildByName("revivelayer");
		g_gameLayer->removeChildByName("fightlayer");

		g_gameLayer->heroRevive();
	}
	for (unsigned int i = 0; i < MyPackage::vec_packages.size(); i++)
	{
		if (MyPackage::vec_packages[i].strid.compare("73") == 0)
		{
			MyPackage::cutone("73");
			return;
		}
	}

	std::map<int, std::vector<PackageData>>::iterator it;
	for (it = StorageRoom::map_storageData.begin(); it != StorageRoom::map_storageData.end(); ++it)
	{
		for (unsigned int i = 0; i < StorageRoom::map_storageData[it->first].size(); i++)
		{
			if (StorageRoom::map_storageData[it->first][i].strid.compare("73") == 0)
			{
				StorageRoom::use("73");
				return;
			}
		}
	}
}