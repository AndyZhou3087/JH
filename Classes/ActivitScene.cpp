#include "ActivitScene.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "Const.h"
#include "GlobalData.h"

bool ActivitScene::ispoping = false;
ActivitScene::ActivitScene()
{

}


ActivitScene::~ActivitScene()
{
	ispoping = false;
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
	if (imagepath.compare("images/cday.jpg") == 0 && content.compare(CommonFuncs::gbk2utf("黑夜降临...")) == 0)
	{
		Sprite* night = Sprite::create("images/cdaynight.png");
		night->setPosition(image->getPosition());
		this->addChild(night);
	}
	cocos2d::ui::Text* tips = (cocos2d::ui::Text*)csbnode->getChildByName("tips");
	int r = GlobalData::createRandomNum(sizeof(tipswords) / sizeof(tipswords[0]));
	std::string str = StringUtils::format("tips：%s", tipswords[r].c_str());
	tips->setString(CommonFuncs::gbk2utf(str.c_str()));

	this->scheduleOnce(schedule_selector(ActivitScene::popself), 2.0f);
	if (ispoping)
		this->scheduleOnce(schedule_selector(ActivitScene::popself), 1.0f);
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
	ispoping = true;
	Director::getInstance()->popScene();
}

