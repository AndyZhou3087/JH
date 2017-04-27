#include "SelectHeroScene.h"
#include "GlobalData.h"
#include "HintBox.h"
#include "CommonFuncs.h"
#include "GameDataSave.h"
#include "GameScene.h"
#include "BuyDetailsLayer.h"
USING_NS_CC;

std::string heroname[] = { "小虾米", "项少龙", "小鱼儿", "阿青" };
std::string herodesc[] = { "哈哈哈哈哈", "太帅了太帅了太帅了", "厉害了我的哥厉害了我的哥厉害了我的哥", "好厉害的说好厉害的说好厉害的说好厉害的说好厉害的说" };
int heroprice[] = { 0, 2, 4, 6 };
SelectHeroScene* g_SelectHeroScene = NULL;
SelectHeroScene::SelectHeroScene()
{
	_lastSelect = 1;
	isMoving = false;
}
SelectHeroScene::~SelectHeroScene()
{
	g_SelectHeroScene = NULL;
}

Scene* SelectHeroScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	g_SelectHeroScene = SelectHeroScene::create();

    // add layer as a child to scene
	scene->addChild(g_SelectHeroScene);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SelectHeroScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	GlobalData::loadUnlockHeroData();

	Node* csbnode = CSLoader::createNode("selectHeroLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* startbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("startbtn");
	startbtn->addTouchEventListener(CC_CALLBACK_2(SelectHeroScene::onStart, this));

	for (int i = 0; i < 4; i++)
	{
		std::string str = StringUtils::format("heroimg%d", i + 1);
		heroimg[i] = (cocos2d::ui::ImageView*)csbnode->getChildByName(str);
		heroimg[i]->setTag(i + 1);
		heroimg[i]->addTouchEventListener(CC_CALLBACK_2(SelectHeroScene::onSelect, this));
		heroimg[i]->setLocalZOrder(i);

		str = StringUtils::format("select_%d", i + 1);
		selectimg[i] = (cocos2d::ui::ImageView*)heroimg[i]->getChildByName(str);
		selectimg[i]->setPositionX(heroimg[i]->getContentSize().width / 2);
		heronameimg[i] = (cocos2d::ui::Widget*)selectimg[i]->getChildByName("sheroname");
		heronameimg[i]->setPositionX(selectimg[i]->getContentSize().width / 2);

		str = StringUtils::format("lock_%d", i + 1);
		lock[i] = (cocos2d::ui::ImageView*)heroimg[i]->getChildByName(str);
		lock[i]->setVisible(!GlobalData::getUnlocHero(i));
	}
	int defaultindex = 0;
	std::string str = StringUtils::format("images/shero%d.jpg", defaultindex + 1);
	heroimg[defaultindex]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	heroimg[defaultindex]->setContentSize(Sprite::create(str)->getContentSize());

	str = StringUtils::format("ui/select.png");
	selectimg[defaultindex]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	selectimg[defaultindex]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
	selectimg[defaultindex]->getChildByName("sheroname")->setPositionX(selectimg[defaultindex]->getContentSize().width / 2);
    return true;
}

void SelectHeroScene::onExit()
{
	Layer::onExit();
}

void SelectHeroScene::onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btnnode = (Node*)pSender;
		int tag = btnnode->getTag();

		if (!GlobalData::getUnlocHero(tag - 1))
		{
			std::string imagpath = StringUtils::format("ui/tophero%d.png", tag);
			Layer* layer = BuyDetailsLayer::create(imagpath, heroname[tag-1], herodesc[tag-1], heroprice[tag-1]);
			layer->setTag(tag - 1);
			this->addChild(layer, 0, "buyherolayer");
			return;
		}
		if (_lastSelect == tag || isMoving)
			return;

		isMoving = true;
		std::string str = StringUtils::format("images/shero%d.jpg", tag);
		heroimg[tag - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		heroimg[tag - 1]->setContentSize(Sprite::create(str)->getContentSize());

		str = StringUtils::format("ui/select.png");
		selectimg[tag - 1]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		selectimg[tag - 1]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		selectimg[tag - 1]->setPositionX(heroimg[tag - 1]->getContentSize().width / 2);

		heronameimg[tag - 1]->setPositionX(selectimg[tag - 1]->getContentSize().width / 2);

		str = StringUtils::format("images/shero%d_s.jpg", _lastSelect);
		heroimg[_lastSelect - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		heroimg[_lastSelect - 1]->setContentSize(Sprite::create(str)->getContentSize());
		heroimg[_lastSelect - 1]->setLocalZOrder(_lastSelect - 1);

		str = StringUtils::format("ui/unselect.png");
		selectimg[_lastSelect - 1]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		selectimg[_lastSelect - 1]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
		selectimg[_lastSelect - 1]->setPositionX(heroimg[_lastSelect - 1]->getContentSize().width / 2);
		heronameimg[_lastSelect - 1]->setPositionX(selectimg[_lastSelect - 1]->getContentSize().width / 2);
		
		int headsize = sizeof(heroimg) / sizeof(heroimg[0]);

		int leftperwidth = (720 - selectimg[tag - 1]->getContentSize().width) /(headsize - 1);
		for (int i = 0; i < headsize; i++)
		{
			int movex = 0;
			if (i < tag - 1)
			{
				movex = leftperwidth / 2 + i*leftperwidth;
			}
			else if (i == tag - 1)
			{
				movex = (tag - 1) * leftperwidth + heroimg[tag - 1]->getContentSize().width / 2;
			}
			else
			{
				movex = (tag - 1)* leftperwidth + heroimg[tag - 1]->getContentSize().width + leftperwidth / 2 + (i - tag) * leftperwidth;
			}
			heroimg[i]->runAction(MoveTo::create(0.1f, Vec2(movex, heroimg[i]->getPositionY())));
			
		}
		this->scheduleOnce(schedule_selector(SelectHeroScene::clickMoveFinish), 0.1f);
		_lastSelect = tag;
	}
}

void SelectHeroScene::onStart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int systime = GlobalData::getSysSecTime();
		std::string uidstr = StringUtils::format("%d", systime);
		GlobalData::setUId(uidstr);
		GameDataSave::getInstance()->setHeroId(_lastSelect);
		Scene* scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void SelectHeroScene::unlockSucc(int index)
{
	this->removeChildByName("buyherolayer");
	lock[index]->setVisible(false);
	GlobalData::setUnlockHero(index, true);
}

void SelectHeroScene::clickMoveFinish(float dt)
{
	isMoving = false;
}

