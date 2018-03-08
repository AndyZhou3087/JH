
#include "NewerGuide2Layer.h"
#include "GlobalData.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GameScene.h"

std::string wordText[][8] = { 
{"江湖行走的还顺利吗？","还好，没遇到什么难事！", "告诉你个小秘密，江湖中的每个好友都是可以结交的~", "那结交后又有什么好处呢？不会要我天天帮他们擦屁股吧~~","结交后可以获得属性的加成或不定期给你送资源，但是想要拉近跟这些江湖人事的关系，还是要付出点代价的。","这个我懂，有钱能使鬼推磨嘛！","哈哈哈，孺子可教！不光是银两，送什么东西都可以，好友度达到两心就可以结交了！", "好，懂了！告辞！"}, 
{ "且慢！", "怎么了？我看他不爽！我要收拾收拾他！", "你这是不要命啊！没看他西欧上显示着强吗？", "强？这是什么意思？", "显示强，就是说这个家伙现在比你厉害好多，等你实力提高了，他就会显示“一般”或者“弱”了", "意思是我现在去找他麻烦就是自讨苦吃？", "没错！等到显示“一般”或者“弱”的时候再去教育他，胜算会高很多！", "好！多谢指点！告辞！"},
{ "想不到这么快你就获得其它侠客的武功了", "那是自然！", "我来教你武功组合吧，可以组合成神功哦！", "武功组合？那是个什么东西？", "顾名思义，就是两个或多个不同的武功可以组合成一个新的武功！但是千万不要随意尝试，小心走火入魔！", "这么厉害？那快告诉我怎么组合！", "首先你要把两个以上的武功升到满级，然后在武功组合界面，主武功下选择武功，副武功处放置武功，然后点击【确认组合】，组合成功后，把主武功装备到你的武功栏就可以使用了！", "原来如此！这么简单？好！那我就先去练功了" },
{ "年轻人！你的马都要饿死了！", "啊？饿死了？那怎么办？", "当然是喂草啊！", "喂草？怎么喂？", "去后山收集草，然后在仓库点击就可以喂啦！", "这么麻烦？没有好点的马吗？", "当然有啦！商城有宝马再卖，那些宝马都是不用吃草的名驹！等你有钱了就换一匹吧！", "好的！多谢！" },
{ "稍等！你刚刚接受了这个人的任务，接受任务后都会出现这个任务计时清单！", "啊？是吗？那要怎么办？", "任务一般分为寻物、寻人、挑战几种任务。", "那分别要怎么才能完成？", "寻人和挑战就简单了，只要找对应的人去对话或者战胜对方就可以。", "那这个寻物呢？", "这个寻物就要去寻找指定物品，然后通过赠送的方式送给他，他自然会给你相应的报酬！切记要在任务时间内完成！", "好的！多谢！" }
};

NewerGuide2Layer::NewerGuide2Layer()
{
	wordindex = 0;
}
NewerGuide2Layer::~NewerGuide2Layer()
{

}

NewerGuide2Layer* NewerGuide2Layer::create(int type, std::vector<Node*> stencilNodes)
{
	NewerGuide2Layer *pRet = new NewerGuide2Layer();
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

bool NewerGuide2Layer::init(int step, std::vector<Node*> stencilNodes)
{
	if (!Layer::init())
	{
		return false;
	}

	if (step != 104)
	{
		auto colorlayer = LayerColor::create(Color4B(0, 0, 0, 200));
		this->addChild(colorlayer);
	}

	m_step = step;
	m_stencilNodes = stencilNodes;
	Node* csbnode = CSLoader::createNode("npctalkLayer.csb");
	this->addChild(csbnode, 2);

	Node* talknode = (Node*)csbnode->getChildByName("node");
	cocos2d::ui::Widget* newerguider = (cocos2d::ui::Widget*)talknode->getChildByName("npcimg");
	newerguider->setOpacity(150);
	cocos2d::ui::ImageView* heroimg = (cocos2d::ui::ImageView*)talknode->getChildByName("heroimg");

	std::string heroidstr = StringUtils::format("ui/fhero%d.png", g_hero->getHeadID());
	heroimg->loadTexture(heroidstr, cocos2d::ui::TextureResType::PLIST);

	wordtext = (cocos2d::ui::Text*)csbnode->getChildByName("node")->getChildByName("text");

	wordtext->setVisible(false);

	m_wordlbl = Label::createWithTTF("", "fonts/STXINGKA.TTF", 26);
	m_wordlbl->setColor(Color3B(0, 0, 0));
	m_wordlbl->setLineBreakWithoutSpace(true);
	m_wordlbl->setMaxLineWidth(610);
	m_wordlbl->setPosition(Vec2(0, -115));
	talknode->addChild(m_wordlbl);

	if (m_step == 100 || m_step == 102)
	{
		csbnode->getChildByName("node")->setPositionY(800);
	}
	else if (m_step == 104 && wordindex == 0)
	{
		hightNode(m_stencilNodes);
	}
	showWords();
	//layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		wordindex++;
		if (wordindex % 2 == 0)
		{
			heroimg->setOpacity(150);
			newerguider->setOpacity(255);
		}
		else
		{
			heroimg->setOpacity(255);
			newerguider->setOpacity(150);
		}

		if (wordindex >= 8)
		{
			GameDataSave::getInstance()->setIsNewerGuide(m_step, 0);
			this->removeFromParentAndCleanup(true);
			return true;
		}
		else if (wordindex == 2 && m_step == 100)
		{
			std::vector<Node*> vec_snode;
	
			vec_snode.push_back(m_stencilNodes[0]);
			hightNode(vec_snode);
		}
		else if (wordindex == 4 && m_step == 100)
		{
			std::vector<Node*> vec_snode;

			vec_snode.push_back(m_stencilNodes[1]);
			hightNode(vec_snode);
		}

		else if (wordindex == 6 && m_step == 100)
		{
			std::vector<Node*> vec_snode;
			for (unsigned int i = 2; i < m_stencilNodes.size(); i++)
			{
				vec_snode.push_back(m_stencilNodes[i]);
			}
			hightNode(vec_snode);
		}
		else if (m_step == 101 && wordindex == 2)
		{
			hightNode(m_stencilNodes);
		}
		else if (m_step == 102 && wordindex == 6)
		{
			hightNode(m_stencilNodes);
		}
		showWords();
		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool NewerGuide2Layer::checkifNewerGuide(int index)
{
	return GameDataSave::getInstance()->getIsNewerGuide(index);
}

void NewerGuide2Layer::showWords()
{
	std::string wordstr = wordText[m_step - 100][wordindex];
	//wordtext->setString(CommonFuncs::gbk2utf(wordstr.c_str()));

	std::string utf8word = CommonFuncs::gbk2utf(wordstr.c_str());
	m_wordlbl->setString(utf8word);

	std::vector<std::string> vec_resname;
	vec_resname.push_back(CommonFuncs::gbk2utf("赠送"));
	vec_resname.push_back(CommonFuncs::gbk2utf("确认组合"));

	std::vector<std::string>::iterator it;
	for (unsigned int i = 0; i < vec_resname.size(); i++)
	{
		std::string resname = vec_resname[i];
		std::size_t findpos = utf8word.find(resname);
		if (findpos != std::string::npos)
		{
			int sindex = findpos / 3;
			int len = resname.size() / 3;
			for (int i = sindex; i < sindex + len; i++)
			{
				m_wordlbl->getLetter(i)->setColor(Color3B(204, 4, 4));
			}
		}
	}
}

void NewerGuide2Layer::hightNode(std::vector<Node*> sNodes)
{
	Node* s_stencil = this->getChildByName("stencil");
	if (s_stencil != NULL)
		s_stencil->removeFromParentAndCleanup(true);
	if (sNodes.size() > 0)
	{
		Node* stencil = Node::create();

		if (m_step == 104)
		{
			ClippingNode * clippingNode = ClippingNode::create();
			clippingNode->setInverted(true);//设置底板可见
			clippingNode->setAlphaThreshold(0.5f);//设置透明度Alpha值为0
			this->addChild(clippingNode, 1, "stencil");
			LayerColor * m_colorlayer = LayerColor::create(Color4B(0, 0, 0, 200));
			clippingNode->addChild(m_colorlayer);
			clippingNode->setStencil(stencil);
		}
		else
		{
			this->addChild(stencil, 1, "stencil");
		}


		for (unsigned int i = 0; i < sNodes.size(); i++)
		{
			std::string str = StringUtils::format("ui/%s.png", (const char*)sNodes[i]->getUserData());
			Sprite* cnode = Sprite::createWithSpriteFrameName(str);
			cnode->setPosition(sNodes[i]->getParent()->convertToWorldSpace(sNodes[i]->getPosition()));
			cnode->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.6f), FadeIn::create(0.6f), NULL)));
			if (m_step == 100 && wordindex == 6)
				cnode->setScale(0.6f);
			else if (m_step == 100 && wordindex == 2)
			{
				Label *textlbl = Label::createWithTTF(CommonFuncs::gbk2utf("结交"), "fonts/STXINGKA.TTF", 35);
				textlbl->setPosition(Vec2(cnode->getContentSize().width / 2, cnode->getContentSize().height / 2));
				cnode->addChild(textlbl);
			}
			else if (m_step == 100 && wordindex == 4)
			{
				Label *textlbl = Label::createWithTTF(CommonFuncs::gbk2utf("赠送"), "fonts/STXINGKA.TTF", 35);
				textlbl->setPosition(Vec2(cnode->getContentSize().width / 2, cnode->getContentSize().height / 2));
				cnode->addChild(textlbl);
			}
			stencil->addChild(cnode);
		}
	}
}