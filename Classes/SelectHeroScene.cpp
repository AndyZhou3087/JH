#include "SelectHeroScene.h"

USING_NS_CC;

SelectHeroScene::SelectHeroScene()
{
	_lastSelect = 1;
}
SelectHeroScene::~SelectHeroScene()
{

}

Scene* SelectHeroScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	SelectHeroScene* startlayer = SelectHeroScene::create();

    // add layer as a child to scene
	scene->addChild(startlayer);

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

	Node* csbnode = CSLoader::createNode("selectHeroLayer.csb");
	this->addChild(csbnode);

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
		heroname[i] = (cocos2d::ui::Widget*)selectimg[i]->getChildByName("sheroname");
		heroname[i]->setPositionX(selectimg[i]->getContentSize().width / 2);
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
		if (_lastSelect == tag)
			return;

		std::string str = StringUtils::format("images/shero%d.jpg", tag);
		heroimg[tag - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		heroimg[tag - 1]->setContentSize(Sprite::create(str)->getContentSize());
		heroimg[tag - 1]->setLocalZOrder(10);

		str = StringUtils::format("ui/select.png");
		selectimg[tag - 1]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		selectimg[tag - 1]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		selectimg[tag - 1]->setPositionX(heroimg[tag - 1]->getContentSize().width / 2);

		heroname[tag - 1]->setPositionX(selectimg[tag - 1]->getContentSize().width / 2);

		str = StringUtils::format("images/shero%d_s.jpg", _lastSelect);
		heroimg[_lastSelect - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		heroimg[_lastSelect - 1]->setContentSize(Sprite::create(str)->getContentSize());
		heroimg[_lastSelect - 1]->setLocalZOrder(_lastSelect - 1);

		str = StringUtils::format("ui/unselect.png");
		selectimg[_lastSelect - 1]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		selectimg[_lastSelect - 1]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
		selectimg[_lastSelect - 1]->setPositionX(heroimg[_lastSelect - 1]->getContentSize().width / 2);
		heroname[_lastSelect - 1]->setPositionX(selectimg[_lastSelect - 1]->getContentSize().width / 2);

		_lastSelect = tag;
	}
}

