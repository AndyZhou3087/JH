
#include "NewerGuideLayer.h"
#include "GlobalData.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"

std::string descText[] = {"在这里可查看建造所需要的资源", "点击材料即可查看材料的详细信息", "点击角色头像即可查看角色属性及更换装备等操作", "点击武功即可选择武功进行使用",
"", "点击内功即可选择内功进行使用", "", "点击武器即可选择武器进行使用",
"", "点击装备即可选择护甲进行使用", "", "",
"","点击角色属性图标即可查看属性状态，每个属性都会对角色造成不同的影响，要注意不让各种状态过低", "", "", 
"","", "出门找点造床的材料", "", 
"移动地点时根据距离不同消耗的时间也有不同", "后山资源丰富，现在就出发", "", "做床需要铁块，先采两块铁矿",
"获得一块铁矿，放到背包中", "每次只能采一块，再采一块", "可以全部拾取哦~~", "",
"还需要点木材，砍两根松树", "获得一根木材，放到背包中", "每次只能采一根，再采一根", "可以全部拾取哦~~",
"", "还需要两块兽皮，打猎狼可以获取狼皮。战斗时不但能获得物品，还可获得经验值，当经验值满足条件后角色即可升级，武学也会自动升级，等级越高越厉害", "获得一块狼皮放到背包中", "",
"还需要一块狼皮，继续打猎狼", "可以全部拾取哦~~","", "", 
"材料收集齐了，可以回家造一张舒适的床了^^","","", "在床上休息可恢复已消耗的生命值", 
"被冰冻图案无法进行消除，必须先消除相邻的其它图案才能破碎冰块。", "被边框圈起来的图案组无法进行最外围的消除，只能从内部消除。", "打败小怪兽就能得到丰富的奖励，加油吧，我的小勇士！", "经典挑战，挑战你的极限！",
"天下武功，唯快不破，勇士敢不敢来挑战一下！"
};

NewerGuideLayer* g_NewerGuideLayer = NULL;
NewerGuideLayer::NewerGuideLayer()
{

}
NewerGuideLayer::~NewerGuideLayer()
{
	g_NewerGuideLayer = NULL;
}

NewerGuideLayer* NewerGuideLayer::create(int type, std::vector<Node*> stencilNodes)
{
	NewerGuideLayer *pRet = new NewerGuideLayer();
	if (pRet && pRet->init(type, stencilNodes))
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

bool NewerGuideLayer::init(int step, std::vector<Node*> stencilNodes)
{
	if (!Layer::init())
	{
		return false;
	}
	m_step = step;
	if (stencilNodes.size() > 0)
	{
		m_clippingNode = ClippingNode::create();
		m_clippingNode->setInverted(true);//设置底板可见
		m_clippingNode->setAlphaThreshold(0.0f);//设置透明度Alpha值为0
		this->addChild(m_clippingNode, 1);

		m_colorlayer = LayerColor::create(Color4B(0, 0, 0, 200));
		m_clippingNode->addChild(m_colorlayer);

		Node* stencil = Node::create();
		for (unsigned int i = 0; i < stencilNodes.size(); i++)
		{
			Sprite* cnode = Sprite::create("images/newerguide/newerwhite.jpg");
			cnode->setPosition(stencilNodes[i]->getParent()->convertToWorldSpace(stencilNodes[i]->getPosition()));
			float scalex = stencilNodes[i]->getContentSize().width / cnode->getContentSize().width;
			float scaley = stencilNodes[i]->getContentSize().height / cnode->getContentSize().height;
			cnode->setScale(scalex, scaley);

			stencil->addChild(cnode);
		}
		m_clippingNode->setStencil(stencil);

		showAnim(stencilNodes[stencilNodes.size() - 1]->getParent()->convertToWorldSpace(stencilNodes[stencilNodes.size() - 1]->getPosition()));
	}

	if (descText[step].length() > 0)
	{
		Sprite* textbox = Sprite::create("images/newerguide/newertextbox.png");

		if (step == 2 || step == 13 || step == 18 || step == 21 || step == 25 || step == 26 || step == 28 || step == 30 || step == 31 || step == 34 || step == 35 || step == 37 || step == 38)
			textbox->setPosition(Vec2(360, 650));
		else
			textbox->setPosition(Vec2(360, 170));
		this->addChild(textbox, 2);

		Label *wordlbl = Label::createWithTTF(CommonFuncs::gbk2utf(descText[step].c_str()), "fonts/STXINGKA.TTF", 25);
		wordlbl->setLineBreakWithoutSpace(true);
		wordlbl->setMaxLineWidth(610);
		wordlbl->setColor(Color3B(0, 0, 0));
		wordlbl->setPosition(textbox->getContentSize().width / 2, textbox->getContentSize().height / 2);
		textbox->addChild(wordlbl);
	}

	//layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		if (stencilNodes.size() > 0)
		{
			auto point = Director::getInstance()->convertToGL(touch->getLocationInView());//获得当前触摸的坐标 
			Vec2 vec = stencilNodes[stencilNodes.size() - 1]->getParent()->convertToWorldSpace(stencilNodes[stencilNodes.size() - 1]->getPosition());

			auto rect = Rect(vec.x - stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.width / 2, vec.y - stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.height/2, stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.width, stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.height);
			if (rect.containsPoint(point))//如果触点处于rect中  
			{
				listener->setSwallowTouches(false);
			}
			else
			{
				listener->setSwallowTouches(true);
			}
		}
		else
		{
			listener->setSwallowTouches(true);
		}
		return true;
	};

	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		if (stencilNodes.size() > 0)
		{
			auto point = Director::getInstance()->convertToGL(touch->getLocationInView());//获得当前触摸的坐标  
			Vec2 vec = stencilNodes[stencilNodes.size() - 1]->getParent()->convertToWorldSpace(stencilNodes[stencilNodes.size() - 1]->getPosition());

			auto rect = Rect(vec.x - stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.width / 2, vec.y - stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.height / 2, stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.width, stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.height);
			if (rect.containsPoint(point))//如果触点处于rect中  
			{
				this->removeFromParentAndCleanup(true);
			}
		}
		else
		{
			this->removeFromParentAndCleanup(true);
		}
		GameDataSave::getInstance()->setIsNewerGuide(step, 0);
	};


	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool NewerGuideLayer::checkifNewerGuide(int index)
{
	return GameDataSave::getInstance()->getIsNewerGuide(index);
}

void NewerGuideLayer::showAnim(Vec2 pos)
{
	Sprite* quan = Sprite::create("images/newerguide/newerquan.png");
	quan->setPosition(pos);
	this->addChild(quan, 1);

	auto sj = Sprite::create("images/newerguide/newersj0.png");
	sj->setAnchorPoint(Vec2(0, 1));
	sj->setPosition(pos.x + 20, pos.y - 20);
	this->addChild(sj, 1);

	//创建帧动画序列，名词形式
	auto animation = Animation::create();
	for (int i = 0; i< 2; i++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "images/newerguide/newersj%d.png", i);
		animation->addSpriteFrameWithFile(szName);
	}
	//设置帧动画属性
	animation->setDelayPerUnit(2.0f / 6);//每一帧停留的时间
	animation->setRestoreOriginalFrame(true);//播放完后回到第一帧
	auto animate = Animate::create(animation);
	sj->runAction(RepeatForever::create(animate));

	if (m_step == 0 || m_step == 43)
	{
		auto hammer = Sprite::create("images/newerguide/newerhammer0.png");
		hammer->setPosition(pos.x + 20, pos.y + 20);
		this->addChild(hammer);

		//创建帧动画序列，名词形式
		auto animation1 = Animation::create();
		for (int i = 0; i < 2; i++)
		{
			char szName[100] = { 0 };
			sprintf(szName, "images/newerguide/newerhammer%d.png", i);
			animation1->addSpriteFrameWithFile(szName);
		}
		//设置帧动画属性
		animation1->setDelayPerUnit(2.0f / 6);//每一帧停留的时间
		animation1->setRestoreOriginalFrame(true);//播放完后回到第一帧
		auto animate1 = Animate::create(animation1);
		hammer->runAction(RepeatForever::create(animate1));
	}

}