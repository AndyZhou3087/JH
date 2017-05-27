
#include "NewerGuideLayer.h"
#include "GlobalData.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"

std::string descText[] = {"在这里可查看建造所需要的资源", "点击材料即可查看材料的详细信息", "点击角色头像即可查看角色属性及更换装备等操作", "点击武功即可选择武功进行使用",
"", "点击内功即可选择内功进行使用", "", "点击武器即可选择武器进行使用",
"", "点击装备即可选择护甲进行使用", "", "",
"每天来签到领取丰富小礼物，连续签到七天赢取超级大礼。","遇到小怪兽千万不要害怕，但是要注意小怪兽施放的法术，打败他，嘿嘿嘿！", "找不到消除的图案了吗？不要慌！使用提示道具试一试。", "又找不到消除的图案了吗？不要慌！使用重排道具试一试。", 
"这是加时道具，使用它可以增加15秒的闯关时间","快来和其他小伙伴一决高低下吧！前三名可是会获得钻石奖励的哦~", "好了，让我们抓紧时间继续前进吧。", "时间结束了？没关系，使用加时道具试一试。", 
"呜呜呜~~~挑战失败了，我可不想重新开始，让我们再来一次吧。", "现在可以进入成长界面，学习新的技能了哦~", "这里有六种技能可以提升你的实力。", "凶恶的小怪兽守护着无穷无尽的宝藏，打败小怪兽，宝藏抱回家！",
"这就是游戏界面了，让我给你介绍一下基础游戏规则。", "看这2个闪闪发亮的图标，来试试滑动消除它们。", "太棒了，当局面较为复杂时，使用提示道具能帮你节省很多时间。", "看，所有的图标都重新排列了，这样我们能更容易找到可以消除的图形。",
"有时候离过关就差一点点，加时道具能帮你顺利过关哦。", "太棒了，这后面还有好多技能，我们一起来看看。", "每次消除都有几率加时，太棒了，但是我们需要通关第二幕才能领悟。", "后面还有更多有用的技能，让我们继续努力前进吧。",
"看，这里有个宝箱在闪闪发光，点进去看看。", "过关获得的星星累计到一定数量就能领取奖励，这是我们的第一个奖励,真是太棒了。", "后面还有更多的奖励，我们继续努力吧。", "这是我们通关第一幕后解锁的道具，来试试看吧。",
"此种石块无法进行消除，会阻挡你的消除路线。", "此种石块只要把宝石消除掉就可进行消除。","", "云会不定时散开，你要找机会记住被遮挡住的是什么图案。", 
"点击问号牌即可翻开查看，但是你点击其它图案时又会翻过去哦。","被锁住的图案是无法进行消除的，你要先找到对应的钥匙进行消除才能解锁哦。","", "移动要按照数字的顺序进行消除才能进行消除哦。", 
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
		textbox->setPosition(Vec2(360, 300));
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
		//GameDataSave::getInstance()->setIsNewerGuide(step, 0);
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

	if (m_step == 0)
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