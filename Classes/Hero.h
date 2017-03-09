#ifndef _HERO_H_
#define _HERO_H_
#include "cocos2d.h"
USING_NS_CC;

class Hero:public Node
{
public:
	Hero();
	~Hero();
	virtual bool init();

	CREATE_FUNC(Hero);

	CC_SYNTHESIZE(int, m_pic, HeadImg);
	CC_SYNTHESIZE(int, m_innerinjury, InnerinjuryValue);
	CC_SYNTHESIZE(int, m_outinjury, OutinjuryValue);
	CC_SYNTHESIZE(int, m_hunger, HungerValue);
	CC_SYNTHESIZE(float, m_spirit, SpiritValue);
	CC_SYNTHESIZE(int, m_life, LifeValue);
	CC_SYNTHESIZE(int, m_maxlife, MaxLifeValue);
	void sleep(int hour);
	void drinking();
public:
	static int MAXInnerinjuryValue;
	static int MAXOutinjuryValue;
	static int MAXHungerValue;
	static int MAXSpiritValue;
private:
	void updateData(float dt);
	void sleepbystep(float dt);
	void drinkbystep(float dt);
private:
	int sleephour;
};
static std::string innerInjurydesc1[] = { "六脉调和", "脉络贯通", "舒筋活络", "内息混乱", "经脉错乱", "经脉寸断", "命不久已" };
static std::string innerInjurydesc = {"内伤严重时会降低生命值上限，要时刻注意内伤状态，使用药物可治疗内伤。"};

static std::string outInjurydesc1[] = { "生龙活虎", "身强力壮", "安危相易", "体无完肤", "皮伤肉绽", "遍体鳞伤", "命不久已" };
static std::string outInjurydesc = { "外伤严重时会降低生命值上限，要时刻注意外伤状态，使用药物可治疗外伤。" };

static std::string hungerdesc1[] = { "鼓腹含和", "酒足饭饱", "啜菽饮水", "饥不择食", "饥肠辘辘", "忍饥受饿", "命不久已" };
static std::string hungerdesc = { "过度饥饿时会加速其它属性的消耗，并降低攻击力与防御力的效果。适当饮食可保持良好状态。" };

static std::string spiritInjurydesc1[] = { "意气风发", "朝气蓬勃", "神气十足", "垂头丧气", "没精打采", "萎靡不振", "生无可恋" };
static std::string spiritInjurydesc = { "精神状态不好时会降低所有属性效果及恢复效率，保持良好的精神状态很重要。喝酒或打败恶人会恢复精神状态。" };

static std::string lifedesc = { "生命值消耗完时角色死亡，通过睡眠可恢复生命。" };
#endif

